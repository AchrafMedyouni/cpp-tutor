import { createHash } from "node:crypto";
import { existsSync } from "node:fs";
import { mkdir, realpath, rename } from "node:fs/promises";
import path from "node:path";
import { fileURLToPath } from "node:url";
import type { CompileResult, Diagnostic, RunOutput, SanitizerMode, SourceFile } from "../../../shared/api.js";
import { getCapabilities } from "./detect.js";
import { cleanPaths, detectLeak, detectSanitizer, parseDiagnostics } from "./diagnostics.js";
import { exec, LIMITS, validateFileName, writeFiles } from "./sandbox.js";

const here = path.dirname(fileURLToPath(import.meta.url));
export const SUPPORT_DIR = path.resolve(here, "../../cpp-support");
export const REPO_ROOT = path.resolve(here, "../../..");
const CACHE_DIR = path.join(REPO_ROOT, ".cache", "objects");

export const BASE_FLAGS = ["-std=c++20", "-Wall", "-Wextra", "-pedantic", "-g", "-pthread"];

export interface BuildOptions {
  sanitizer: SanitizerMode;
  trackAllocs: boolean;
  defines?: string[];
  /** Hidden sources (test harness) compiled with the user's code; never shown as editable. */
  hidden?: SourceFile[];
  /** Link the precompiled doctest main. */
  doctestMain?: boolean;
  /** UB aborts instead of continuing (used for grading). */
  ubAborts?: boolean;
}

export interface BuildResult {
  compile: CompileResult;
  binary?: string;
}

function sanitizerFlags(mode: SanitizerMode, ubAborts: boolean): string[] {
  if (mode === "asan_ubsan")
    return ["-fsanitize=address,undefined", "-fno-omit-frame-pointer", ...(ubAborts ? ["-fno-sanitize-recover=all"] : [])];
  if (mode === "tsan") return ["-fsanitize=thread", "-fno-omit-frame-pointer"];
  return [];
}

const HIDDEN_DIR = "__cpplab";

async function capabilityFlags(allSources: SourceFile[]): Promise<{ compile: string[]; link: string[] }> {
  const caps = await getCapabilities();
  const text = allSources.map((f) => f.content).join("\n");
  const compile: string[] = [];
  const link: string[] = [];
  if (/#\s*include\s*<execution>/.test(text) && caps.par.available) {
    for (const f of caps.par.extraFlags) (f.startsWith("-l") ? link : compile).push(f);
    if (caps.par.extraFlags.includes("-fexperimental-library")) link.push("-fexperimental-library");
  }
  if (/#\s*include\s*<(tbb|oneapi\/tbb)\//.test(text) && caps.tbb.available) {
    compile.push(...caps.tbb.extraFlags);
    link.push(...caps.tbb.linkFlags);
  }
  return { compile, link };
}

/** Compile a support source once per (compiler, flags) and cache the object file. */
async function cachedObject(cxx: string, src: string, flags: string[]): Promise<string> {
  const key = createHash("sha1").update([cxx, src, ...flags].join("\0")).digest("hex").slice(0, 16);
  const out = path.join(CACHE_DIR, `${path.basename(src, ".cpp")}-${key}.o`);
  if (existsSync(out)) return out;
  await mkdir(CACHE_DIR, { recursive: true });
  const tmp = `${out}.${process.pid}.${Date.now()}.tmp`;
  const r = await exec(cxx, [...flags, "-c", src, "-o", tmp], { cwd: CACHE_DIR, timeoutMs: 120_000 });
  if (r.exitCode !== 0) throw new Error(`Failed to build support object ${src}:\n${r.stderr}`);
  await rename(tmp, out);
  return out;
}

export async function build(dir: string, files: SourceFile[], opts: BuildOptions): Promise<BuildResult> {
  const start = Date.now();
  const caps = await getCapabilities();
  if (!caps.compiler) {
    return { compile: { ok: false, diagnostics: [], raw: caps.error ?? "No compiler", durationMs: 0 } };
  }
  for (const f of files) {
    const err = validateFileName(f.name);
    if (err) return { compile: { ok: false, diagnostics: [], raw: err, durationMs: 0 } };
  }
  const cxx = caps.compiler.path;
  const realDir = await realpath(dir);
  await writeFiles(realDir, files);
  const hidden = opts.hidden ?? [];
  if (hidden.length) await writeFiles(path.join(realDir, HIDDEN_DIR), hidden);

  const sanFlags = sanitizerFlags(opts.sanitizer, !!opts.ubAborts);
  const capFlags = await capabilityFlags([...files, ...hidden]);
  const defines = (opts.defines ?? []).filter((d) => /^[A-Za-z_][A-Za-z0-9_]*(=[A-Za-z0-9_.]*)?$/.test(d)).map((d) => `-D${d}`);
  const commonFlags = [...BASE_FLAGS, ...sanFlags, ...defines, `-I${realDir}`, `-I${SUPPORT_DIR}`];

  const units = [
    ...files.filter((f) => /\.(cpp|cc)$/.test(f.name)).map((f) => f.name),
    ...hidden.filter((f) => /\.(cpp|cc)$/.test(f.name)).map((f) => `${HIDDEN_DIR}/${f.name}`),
  ];
  if (units.length === 0) {
    return { compile: { ok: false, diagnostics: [], raw: "No .cpp file to compile.", durationMs: 0 } };
  }

  // Compile every translation unit separately (as the course's separate-compilation model), in parallel.
  const results = await Promise.all(
    units.map((u, i) =>
      exec(cxx, [...commonFlags, ...capFlags.compile, "-c", u, "-o", `__obj${i}.o`], {
        cwd: realDir,
        timeoutMs: LIMITS.compileTimeoutMs,
      }),
    ),
  );
  let raw = results.map((r) => r.stderr + r.stdout).join("");
  let timedOut = results.some((r) => r.timedOut);
  let ok = results.every((r) => r.exitCode === 0);
  let binary: string | undefined;

  if (ok) {
    const objFlags = [...BASE_FLAGS, ...sanFlags, `-I${SUPPORT_DIR}`];
    // The tracker must be linked FIRST: static objects are initialised in link order and destroyed in
    // reverse, so its exit report then runs after every user global (e.g. a global std::string) is gone.
    const firstObjs: string[] = [];
    const extraObjs: string[] = [];
    try {
      firstObjs.push(await cachedObject(cxx, path.join(SUPPORT_DIR, "cpplab/runtime.cpp"), objFlags));
      if (opts.trackAllocs) firstObjs.push(await cachedObject(cxx, path.join(SUPPORT_DIR, "cpplab/alloc_tracker.cpp"), objFlags));
      if (opts.doctestMain) extraObjs.push(await cachedObject(cxx, path.join(SUPPORT_DIR, "doctest_main.cpp"), objFlags));
    } catch (e) {
      return { compile: { ok: false, diagnostics: [], raw: String(e), durationMs: Date.now() - start } };
    }
    const link = await exec(
      cxx,
      [...BASE_FLAGS, ...sanFlags, ...firstObjs, ...units.map((_, i) => `__obj${i}.o`), ...extraObjs, ...capFlags.link, "-o", "prog"],
      { cwd: realDir, timeoutMs: LIMITS.compileTimeoutMs },
    );
    raw += link.stderr + link.stdout;
    timedOut ||= link.timedOut;
    ok = link.exitCode === 0;
    if (ok) binary = path.join(realDir, "prog");
  }

  raw = cleanPaths(raw, realDir, SUPPORT_DIR);
  if (timedOut) raw += `\n[cpplab] compilation timed out after ${LIMITS.compileTimeoutMs / 1000}s`;
  const visible = new Set(files.map((f) => f.name));
  const diagnostics = parseDiagnostics(raw, visible);
  if (!ok && !diagnostics.some((d) => d.severity === "error")) diagnostics.push(...linkerDiagnostics(raw));
  return {
    compile: { ok, diagnostics, raw, durationMs: Date.now() - start, timedOut },
    binary,
  };
}

export const RUN_ENV: NodeJS.ProcessEnv = {
  ASAN_OPTIONS: "detect_leaks=0:abort_on_error=0:detect_stack_use_after_return=1:color=never",
  UBSAN_OPTIONS: "print_stacktrace=1:color=never",
  TSAN_OPTIONS: "color=never:second_deadlock_stack=1",
  MallocNanoZone: "0", // silences a macOS ASan warning
};

export async function runBinary(
  dir: string,
  binary: string,
  opts: { stdin?: string; args?: string[]; env?: NodeJS.ProcessEnv },
): Promise<RunOutput> {
  const realDir = await realpath(dir);
  const r = await exec(binary, opts.args ?? [], {
    cwd: realDir,
    stdin: opts.stdin,
    timeoutMs: LIMITS.runTimeoutMs,
    env: { ...RUN_ENV, ...opts.env },
  });
  const stderr = cleanPaths(r.stderr, realDir, SUPPORT_DIR);
  if (r.timedOut) r.stdout += `\n[cpplab] program killed: exceeded ${LIMITS.runTimeoutMs / 1000}s time limit`;
  return {
    ...r,
    stderr,
    sanitizerReport: detectSanitizer(stderr),
    leak: detectLeak(stderr),
  };
}


/** Summarise linker failures (which have no file:line) as diagnostics. */
function linkerDiagnostics(raw: string): Diagnostic[] {
  const out: Diagnostic[] = [];
  // macOS ld: 'Undefined symbols for architecture arm64:\n  "spread(double, double)", referenced from:'
  for (const m of raw.matchAll(/^\s+"(.+?)", referenced from:/gm))
    out.push({ file: "<linker>", line: 0, col: 0, severity: "error", message: `undefined symbol: ${m[1]} (declared and used, but never defined)` });
  // GNU ld: "undefined reference to `spread(double, double)'"
  for (const m of raw.matchAll(/undefined reference to [`']([^'`]+)'/g))
    out.push({ file: "<linker>", line: 0, col: 0, severity: "error", message: `undefined symbol: ${m[1]} (declared and used, but never defined)` });
  for (const m of raw.matchAll(/duplicate symbol '(.+?)'|multiple definition of [`']([^'`]+)'/g))
    out.push({ file: "<linker>", line: 0, col: 0, severity: "error", message: `symbol defined in more than one object file: ${m[1] ?? m[2]}` });
  if (!out.length && raw.trim()) out.push({ file: "<linker>", line: 0, col: 0, severity: "error", message: raw.trim().split("\n")[0] });
  return out;
}
