import type { Diagnostic, SanitizerReport } from "../../../shared/api.js";

// "file:line:col: severity: message [-Wflag]"  (gcc and clang share this format)
const DIAG_RE = /^(.+?):(\d+):(\d+):\s+(fatal error|error|warning|note):\s+(.*?)(?:\s+\[(-W[^\]]+)\])?$/;

/**
 * Parse compiler output into structured diagnostics.
 * Diagnostics located in hidden files (harness, tracker, doctest) are re-attributed to
 * the last user-visible location in the same "include/instantiation" chain when possible,
 * otherwise kept with file = "<hidden>".
 */
export function parseDiagnostics(raw: string, visibleFiles: Set<string>): Diagnostic[] {
  const out: Diagnostic[] = [];
  for (const line of raw.split("\n")) {
    const m = DIAG_RE.exec(line);
    if (!m) continue;
    const [, file, l, c, sev, message, flag] = m;
    const base = file.replace(/^\.\//, "").split("/").pop() ?? file;
    const visible = visibleFiles.has(base) && !file.includes("/cpp-support/");
    out.push({
      file: visible ? base : "<hidden>",
      line: Number(l),
      col: Number(c),
      severity: sev === "fatal error" ? "error" : (sev as Diagnostic["severity"]),
      message,
      flag,
    });
  }
  return out;
}

/** Strip absolute temp-dir and support paths so output is readable. */
export function cleanPaths(raw: string, workDir: string, supportDir: string): string {
  return raw.split(workDir + "/").join("").split(supportDir + "/").join("<cpplab>/");
}

/** First stack frame that lives in the learner's code (not doctest, the tracker or the std library). */
function firstUserFrame(stderr: string): string | undefined {
  const frameRe = /#\d+ 0x[0-9a-f]+ in (.+?) (?:\(.*?\) )?([\w./<>-]+\.(?:cpp|cc|h|hpp)):(\d+)/g;
  let m: RegExpExecArray | null;
  while ((m = frameRe.exec(stderr))) {
    const file = m[2];
    if (/doctest|<cpplab>|alloc_tracker|__cpplab|\/usr\/|c\+\+\/v1/.test(file)) continue;
    const fn = m[1].replace(/\(.*$/, "");
    return `${fn} (${file.split("/").pop()}:${m[3]})`;
  }
  return undefined;
}

export function detectSanitizer(stderr: string): SanitizerReport | undefined {
  const tsan = /WARNING: ThreadSanitizer: ([^\n(]+)/.exec(stderr);
  if (tsan) {
    const where = firstUserFrame(stderr);
    return { kind: "tsan", summary: `ThreadSanitizer: ${tsan[1].trim()}${where ? ` in ${where}` : ""}` };
  }
  const asan = /ERROR: AddressSanitizer: ([\w-]+)/.exec(stderr);
  if (asan) {
    const where = firstUserFrame(stderr);
    if (where) return { kind: "asan", summary: `AddressSanitizer: ${asan[1]} in ${where}` };
    const summ = /SUMMARY: AddressSanitizer: ([^\n]+)/.exec(stderr);
    return { kind: "asan", summary: `AddressSanitizer: ${(summ?.[1] ?? asan[1]).trim()}` };
  }
  const ub = /runtime error: ([^\n]+)/.exec(stderr);
  if (ub) return { kind: "ubsan", summary: `UndefinedBehaviorSanitizer: ${ub[1].trim()}` };
  return undefined;
}

export function detectLeak(stderr: string): { allocations: number } | undefined {
  const m = /\[cpplab\] (\d+) leaked allocation\(s\)/.exec(stderr);
  if (!m) return undefined;
  return { allocations: Number(m[1]) };
}
