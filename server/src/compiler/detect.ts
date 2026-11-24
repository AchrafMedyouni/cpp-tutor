import { execFileSync } from "node:child_process";
import { existsSync } from "node:fs";
import type { Capabilities } from "../../../shared/api.js";
import { exec, withTempDir, writeFiles } from "./sandbox.js";

let cached: Promise<Capabilities> | null = null;

function which(cmd: string): string | null {
  try {
    return execFileSync("/usr/bin/which", [cmd], { encoding: "utf8" }).trim() || null;
  } catch {
    return null;
  }
}

function resolveCompiler(): { path: string; version: string; family: "clang" | "gcc" | "unknown" } | null {
  const candidates = [process.env.CXX, "g++", "clang++"].filter(Boolean) as string[];
  for (const c of candidates) {
    const p = c.includes("/") ? (existsSync(c) ? c : null) : which(c);
    if (!p) continue;
    try {
      const v = execFileSync(p, ["--version"], { encoding: "utf8" }).split("\n")[0].trim();
      const family = /clang/i.test(v) ? "clang" : /g\+\+|gcc|GCC/.test(v) ? "gcc" : "unknown";
      return { path: p, version: v, family };
    } catch {
      continue;
    }
  }
  return null;
}

async function probe(cxx: string, code: string, flags: string[], run = true): Promise<boolean> {
  return withTempDir(async (dir) => {
    await writeFiles(dir, [{ name: "probe.cpp", content: code }]);
    const c = await exec(cxx, ["-std=c++20", ...flags, "probe.cpp", "-o", "probe"], { cwd: dir, timeoutMs: 30_000 });
    if (c.exitCode !== 0) return false;
    if (!run) return true;
    const r = await exec("./probe", [], { cwd: dir, timeoutMs: 10_000 });
    return r.exitCode === 0;
  });
}

const PAR_PROBE = `#include <execution>
#include <algorithm>
#include <vector>
int main(){ std::vector<int> v(1000, 1); std::for_each(std::execution::par, v.begin(), v.end(), [](int& n){ ++n; }); return v[0] == 2 ? 0 : 1; }`;

const TBB_PROBE = `#include <tbb/parallel_for.h>
#include <vector>
int main(){ std::vector<int> v(1000, 1); tbb::parallel_for(std::size_t(0), v.size(), [&](std::size_t i){ v[i] += 1; }); return v[0] == 2 ? 0 : 1; }`;

async function detect(): Promise<Capabilities> {
  const compiler = resolveCompiler();
  if (!compiler) {
    return {
      compiler: null,
      error:
        "No C++ compiler found. Install one (macOS: `xcode-select --install`; Linux: `sudo apt install g++`) or set the CXX environment variable to its path.",
      asanUbsan: false,
      tsan: false,
      par: { available: false, extraFlags: [] },
      tbb: { available: false, extraFlags: [], linkFlags: [] },
    };
  }
  const cxx = compiler.path;
  const trivial = "int main(){ int* p = new int(1); int r = *p - 1; delete p; return r; }";
  const [cxx20, asanUbsan, tsan] = await Promise.all([
    probe(cxx, "#include <compare>\nstruct P{int x; auto operator<=>(const P&) const = default;};\nint main(){ return P{1} < P{2} ? 0 : 1; }", []),
    probe(cxx, trivial, ["-fsanitize=address,undefined"]),
    probe(cxx, trivial, ["-fsanitize=thread"]),
  ]);

  let par = { available: false, extraFlags: [] as string[] };
  if (await probe(cxx, PAR_PROBE, ["-pthread"])) par = { available: true, extraFlags: [] };
  else if (await probe(cxx, PAR_PROBE, ["-pthread", "-fexperimental-library"]))
    par = { available: true, extraFlags: ["-fexperimental-library"] };
  else if (await probe(cxx, PAR_PROBE, ["-pthread", "-ltbb"])) par = { available: true, extraFlags: ["-ltbb"] };

  let tbb = { available: false, extraFlags: [] as string[], linkFlags: [] as string[] };
  const prefixes = ["", "/opt/homebrew", "/usr/local"];
  for (const pre of prefixes) {
    const inc = pre ? [`-I${pre}/include`] : [];
    const lib = pre ? [`-L${pre}/lib`, `-Wl,-rpath,${pre}/lib`, "-ltbb"] : ["-ltbb"];
    if (await probe(cxx, TBB_PROBE, ["-pthread", ...inc, ...lib])) {
      tbb = { available: true, extraFlags: inc, linkFlags: lib };
      break;
    }
  }

  return {
    compiler,
    error: cxx20 ? undefined : `${compiler.version} does not seem to support C++20 (need g++ >= 11 or clang >= 15).`,
    asanUbsan,
    tsan,
    par,
    tbb,
  };
}

export function getCapabilities(): Promise<Capabilities> {
  if (!cached) cached = detect();
  return cached;
}
