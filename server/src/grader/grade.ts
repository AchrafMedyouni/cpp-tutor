import type { GradeResponse, SanitizerMode, SourceFile, TestResult } from "../../../shared/api.js";
import { build, runBinary } from "../compiler/pipeline.js";
import { withTempDir } from "../compiler/sandbox.js";
import type { IoTests } from "../content/schema.js";

function decodeXml(s: string): string {
  return s
    .replace(/&lt;/g, "<")
    .replace(/&gt;/g, ">")
    .replace(/&quot;/g, '"')
    .replace(/&apos;/g, "'")
    .replace(/&amp;/g, "&");
}

/**
 * Parse doctest's XML reporter output. If the program crashed mid-test (sanitizer abort),
 * the last open test case is reported as failed with the crash reason.
 */
export function parseDoctestXml(xml: string, crashReason?: string): TestResult[] {
  const results: TestResult[] = [];
  const caseRe = /<TestCase name="([^"]*)"[^>]*>([\s\S]*?)(<\/TestCase>|$)/g;
  let m: RegExpExecArray | null;
  while ((m = caseRe.exec(xml))) {
    const [, name, body, close] = m;
    const failures: string[] = [];
    const exprRe = /<Expression success="false"([^>]*)>([\s\S]*?)<\/Expression>/g;
    let e: RegExpExecArray | null;
    while ((e = exprRe.exec(body))) {
      const type = /type="([^"]*)"/.exec(e[1])?.[1] ?? "";
      const orig = /<Original>\s*([\s\S]*?)\s*<\/Original>/.exec(e[2])?.[1] ?? "";
      const exp = /<Expanded>\s*([\s\S]*?)\s*<\/Expanded>/.exec(e[2])?.[1];
      const exc = /<Exception[^>]*>\s*([\s\S]*?)\s*<\/Exception>/.exec(e[2])?.[1];
      const expected = /<ExpectedException>\s*([\s\S]*?)\s*<\/ExpectedException>/.exec(e[2])?.[1];
      let msg = decodeXml(orig);
      if (exp && decodeXml(exp) !== msg) msg += `  →  ${decodeXml(exp)}`;
      if (/THROWS_AS/.test(type))
        msg += exc ? `  (threw the wrong exception: ${decodeXml(exc)})` : `  (expected ${expected ? decodeXml(expected) : "an exception"} to be thrown, nothing was thrown)`;
      else if (/NOTHROW/.test(type)) msg += `  (should not throw, but threw${exc ? `: ${decodeXml(exc)}` : ""})`;
      else if (/THROWS/.test(type)) msg += "  (expected an exception, nothing was thrown)";
      else if (exc) msg += `  (threw: ${decodeXml(exc)})`;
      failures.push(msg);
    }
    const infoRe = /<Info>\s*([\s\S]*?)\s*<\/Info>/g;
    const infos: string[] = [];
    let inf: RegExpExecArray | null;
    while ((inf = infoRe.exec(body))) infos.push(decodeXml(inf[1]));
    const exRe = /<Exception crash="(true|false)">\s*([\s\S]*?)\s*<\/Exception>/g;
    let ex: RegExpExecArray | null;
    while ((ex = exRe.exec(body))) failures.push(`uncaught exception: ${decodeXml(ex[2])}`);
    const msgRe = /<Message type="(?:FAIL|ERROR)"[^>]*>[\s\S]*?<Text>\s*([\s\S]*?)\s*<\/Text>/g;
    let mm: RegExpExecArray | null;
    while ((mm = msgRe.exec(body))) failures.push(decodeXml(mm[1]));

    const summary = /<OverallResultsAsserts[^>]*test_case_success="(true|false)"/.exec(body);
    const crashed = !close || close === "";
    const passed = !crashed && summary?.[1] === "true" && failures.length === 0;
    let message: string | undefined;
    if (crashed) message = crashReason ?? "the program crashed during this test";
    else if (!passed) message = failures.join("\n") || "test failed";
    if (message && infos.length) message += `\n(context: ${infos.join("; ")})`;
    results.push({ name: decodeXml(name), passed, message });
    if (crashed) break;
  }
  return results;
}

export async function gradeDoctest(
  userFiles: SourceFile[],
  testFiles: SourceFile[],
  sanitizer: SanitizerMode,
): Promise<GradeResponse> {
  return withTempDir(async (dir) => {
    const { compile, binary } = await build(dir, userFiles, {
      sanitizer,
      trackAllocs: true,
      hidden: testFiles,
      doctestMain: true,
      ubAborts: true,
    });
    if (!compile.ok || !binary) {
      return { compile, tests: [], passed: false, summary: "Compilation failed — fix the errors first." };
    }
    const run = await runBinary(dir, binary, {
      args: ["--reporters=xml", "--no-colors=true", "--no-version=true"],
      env: { CPPLAB_TRACKER_QUIET: "1" },
    });
    const crashReason = run.sanitizerReport
      ? run.sanitizerReport.summary
      : run.timedOut
        ? "timed out (infinite loop or deadlock?)"
        : run.signal
          ? `crashed with signal ${run.signal}`
          : undefined;
    const tests = parseDoctestXml(run.stdout, crashReason);
    const sanitizerProblem = run.sanitizerReport && tests.every((t) => t.passed);
    if (sanitizerProblem) {
      // e.g. TSan reports a race but the asserts happened to pass.
      tests.push({ name: "Sanitizer check", passed: false, message: run.sanitizerReport!.summary });
    }
    if (tests.length === 0) {
      tests.push({
        name: "Test run",
        passed: false,
        message: crashReason ?? (run.stderr.slice(0, 2000) || "no test results (did the program call exit()?)"),
      });
    }
    const passedCount = tests.filter((t) => t.passed).length;
    const passed = passedCount === tests.length;
    return {
      compile,
      run: { ...run, stdout: "" }, // XML is noise for the learner; stderr is kept
      tests,
      passed,
      summary: passed ? `All ${tests.length} tests passed.` : `${passedCount}/${tests.length} tests passed.`,
    };
  });
}

function matches(actual: string, expected: string, mode: "exact" | "trim" | "regex"): boolean {
  if (mode === "exact") return actual === expected;
  if (mode === "regex") return new RegExp(expected).test(actual);
  const norm = (s: string) =>
    s
      .split("\n")
      .map((l) => l.trimEnd())
      .join("\n")
      .trim();
  return norm(actual) === norm(expected);
}

export async function gradeIo(userFiles: SourceFile[], tests: IoTests, sanitizer: SanitizerMode): Promise<GradeResponse> {
  return withTempDir(async (dir) => {
    const { compile, binary } = await build(dir, userFiles, { sanitizer, trackAllocs: false, ubAborts: true });
    if (!compile.ok || !binary) {
      return { compile, tests: [], passed: false, summary: "Compilation failed — fix the errors first." };
    }
    const results: TestResult[] = [];
    let lastRun;
    for (const t of tests) {
      const run = await runBinary(dir, binary, { stdin: t.stdin });
      lastRun = run;
      let passed = matches(run.stdout, t.expectedStdout, t.match) && !run.sanitizerReport && !run.timedOut;
      let message: string | undefined;
      if (run.sanitizerReport) message = run.sanitizerReport.summary;
      else if (run.timedOut) message = "timed out";
      else if (!passed)
        message = `input:\n${t.stdin || "(none)"}\nexpected:\n${t.expectedStdout}\ngot:\n${run.stdout || "(nothing)"}`;
      if (run.exitCode !== 0 && passed) {
        passed = false;
        message = `program exited with code ${run.exitCode}`;
      }
      results.push({ name: t.name, passed, message });
    }
    const n = results.filter((r) => r.passed).length;
    return {
      compile,
      run: lastRun,
      tests: results,
      passed: n === results.length,
      summary: n === results.length ? `All ${n} tests passed.` : `${n}/${results.length} tests passed.`,
    };
  });
}
