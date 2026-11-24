import { spawn } from "node:child_process";
import { mkdtemp, rm, writeFile, mkdir } from "node:fs/promises";
import { tmpdir } from "node:os";
import path from "node:path";
import type { SourceFile } from "../../../shared/api.js";

export const LIMITS = {
  compileTimeoutMs: Number(process.env.CPPLAB_COMPILE_TIMEOUT_MS ?? 20_000),
  runTimeoutMs: Number(process.env.CPPLAB_RUN_TIMEOUT_MS ?? 10_000),
  outputCapBytes: Number(process.env.CPPLAB_OUTPUT_CAP ?? 64 * 1024),
};

const NAME_RE = /^[A-Za-z0-9_][A-Za-z0-9_.-]*\.(cpp|cc|h|hpp)$/;

export function validateFileName(name: string): string | null {
  if (!NAME_RE.test(name)) return `Invalid file name "${name}" (allowed: letters, digits, _ . -, extension .cpp/.cc/.h/.hpp, no directories)`;
  return null;
}

export async function withTempDir<T>(fn: (dir: string) => Promise<T>): Promise<T> {
  const dir = await mkdtemp(path.join(tmpdir(), "cpplab-"));
  try {
    return await fn(dir);
  } finally {
    await rm(dir, { recursive: true, force: true });
  }
}

export async function writeFiles(dir: string, files: SourceFile[]): Promise<void> {
  for (const f of files) {
    const err = validateFileName(f.name);
    if (err) throw new Error(err);
    await mkdir(dir, { recursive: true });
    await writeFile(path.join(dir, f.name), f.content);
  }
}

export interface ExecResult {
  stdout: string;
  stderr: string;
  exitCode: number | null;
  signal: string | null;
  timedOut: boolean;
  truncated: boolean;
  durationMs: number;
}

/** Spawn a process in its own process group, with a timeout and an output cap. */
export function exec(
  cmd: string,
  args: string[],
  opts: { cwd: string; stdin?: string; timeoutMs: number; env?: NodeJS.ProcessEnv; capBytes?: number },
): Promise<ExecResult> {
  const cap = opts.capBytes ?? LIMITS.outputCapBytes;
  return new Promise((resolve) => {
    const start = Date.now();
    const child = spawn(cmd, args, {
      cwd: opts.cwd,
      env: { ...process.env, ...opts.env },
      detached: true,
      stdio: ["pipe", "pipe", "pipe"],
    });
    let out = "";
    let err = "";
    let truncated = false;
    let timedOut = false;
    let done = false;

    const killGroup = () => {
      try {
        if (child.pid) process.kill(-child.pid, "SIGKILL");
      } catch {
        /* already gone */
      }
    };
    const append = (which: "out" | "err", chunk: Buffer) => {
      const cur = which === "out" ? out : err;
      if (cur.length >= cap) {
        if (!truncated) {
          truncated = true;
          // A runaway printer: stop it now rather than waiting for the timeout.
          if (which === "out" && out.length >= cap) killGroup();
        }
        return;
      }
      const next = cur + chunk.toString("utf8");
      const clipped = next.length > cap ? next.slice(0, cap) : next;
      if (next.length > cap) truncated = true;
      if (which === "out") out = clipped;
      else err = clipped;
      if (truncated && which === "out") killGroup();
    };
    child.stdout.on("data", (c: Buffer) => append("out", c));
    child.stderr.on("data", (c: Buffer) => append("err", c));
    child.stdin.on("error", () => {});
    child.stdin.end(opts.stdin ?? "");

    const timer = setTimeout(() => {
      timedOut = true;
      killGroup();
    }, opts.timeoutMs);

    const finish = (code: number | null, signal: NodeJS.Signals | null) => {
      if (done) return;
      done = true;
      clearTimeout(timer);
      if (truncated) {
        if (out.length >= cap) out += `\n[cpplab] output truncated at ${cap} bytes`;
        if (err.length >= cap) err += `\n[cpplab] output truncated at ${cap} bytes`;
      }
      resolve({ stdout: out, stderr: err, exitCode: code, signal, timedOut, truncated, durationMs: Date.now() - start });
    };
    child.on("close", finish);
    child.on("error", (e) => {
      err += String(e);
      finish(null, null);
    });
  });
}
