/**
 * npm run verify-content [-- <chapter-id>... ] [--only=examples,exercises,projects,quizzes,viz] [-j N]
 *
 * Compiles and checks every piece of course content:
 *  - examples: "clean" ones compile with zero warnings and run clean under ASan+UBSan (or TSan),
 *    with no leak and the expected stdout; bug demos must exhibit their bug.
 *  - exercises: the reference solution passes all tests with zero warnings; the starter fails at least one test.
 *  - projects: each step's solution passes that step's tests (and earlier ones when cumulative);
 *    the starter fails step 1.
 *  - quizzes: answers in range; questions with a `verify` block are compiled and checked.
 *  - visualizer scenarios: schema-valid, line numbers in range, refs resolve.
 *  - lessons: every ::example / ::visualize id resolves; every example is used by some lesson.
 */
import { existsSync } from "node:fs";
import { readdir } from "node:fs/promises";
import os from "node:os";
import path from "node:path";
import type { SanitizerMode, SourceFile } from "../shared/api.js";
import { getCapabilities } from "../server/src/compiler/detect.js";
import { build, runBinary } from "../server/src/compiler/pipeline.js";
import { withTempDir } from "../server/src/compiler/sandbox.js";
import {
  CONTENT_DIR,
  listExamples,
  listExercises,
  listProjects,
  loadCourse,
  loadExample,
  loadExerciseInternals,
  loadLesson,
  listLessons,
  loadProjectStepInternals,
  loadProject,
  loadQuiz,
  loadViz,
  listViz,
} from "../server/src/content/loader.js";
import { gradeDoctest, gradeIo } from "../server/src/grader/grade.js";

type Status = "pass" | "fail" | "skip";
interface Result {
  kind: string;
  id: string;
  status: Status;
  detail?: string;
  ms: number;
}

const args = process.argv.slice(2);
const onlyArg = args.find((a) => a.startsWith("--only="))?.slice(7).split(",");
const jIdx = args.indexOf("-j");
const CONCURRENCY = jIdx >= 0 ? Number(args[jIdx + 1]) : Math.max(2, Math.floor(os.cpus().length / 2));
const chapterFilter = args.filter((a, i) => !a.startsWith("-") && args[i - 1] !== "-j");
const want = (k: string) => !onlyArg || onlyArg.includes(k);

const caps = await getCapabilities();
if (!caps.compiler) {
  console.error(caps.error);
  process.exit(2);
}
function missing(req?: string[]): string[] {
  return (req ?? []).filter((r) =>
    r === "par" ? !caps.par.available : r === "tbb" ? !caps.tbb.available : r === "tsan" ? !caps.tsan : r === "asan_ubsan" ? !caps.asanUbsan : false,
  );
}

const tasks: (() => Promise<Result>)[] = [];
const results: Result[] = [];

function task(kind: string, id: string, fn: () => Promise<string | null | { skip: string }>) {
  tasks.push(async () => {
    const t = Date.now();
    try {
      const r = await fn();
      if (r && typeof r === "object") return { kind, id, status: "skip", detail: r.skip, ms: Date.now() - t };
      return { kind, id, status: r ? "fail" : "pass", detail: r ?? undefined, ms: Date.now() - t };
    } catch (e) {
      return { kind, id, status: "fail", detail: e instanceof Error ? e.message : String(e), ms: Date.now() - t };
    }
  });
}

const warningsOf = (c: { diagnostics: { severity: string; file: string; line: number; message: string }[] }) =>
  c.diagnostics.filter((d) => d.severity === "warning").map((d) => `${d.file}:${d.line}: ${d.message}`);
const norm = (s: string) =>
  s
    .split("\n")
    .map((l) => l.trimEnd())
    .join("\n")
    .trim();
const clip = (s: string, n = 600) => (s.length > n ? s.slice(0, n) + "…" : s);

async function compileAndRun(files: SourceFile[], o: { sanitizer: SanitizerMode; stdin?: string; defines?: string[] }) {
  return withTempDir(async (dir) => {
    const { compile, binary } = await build(dir, files, { sanitizer: o.sanitizer, trackAllocs: true, defines: o.defines });
    if (!compile.ok || !binary) return { compile, run: undefined };
    const run = await runBinary(dir, binary, { stdin: o.stdin });
    return { compile, run };
  });
}

// ---------------------------------------------------------------- examples
async function checkExample(ch: string, id: string): Promise<string | null | { skip: string }> {
  const ex = await loadExample(ch, id);
  const m = ex.meta;
  const miss = missing(m.requires);
  if (miss.length) return { skip: `needs ${miss.join(", ")}` };
  if (m.expect === "compile-error") {
    const { compile } = await compileAndRun(ex.files, { sanitizer: "none", defines: m.defines });
    return compile.ok ? "expected a compile error, but it compiled" : null;
  }
  const sanitizer: SanitizerMode =
    m.expect === "race" ? "tsan" : m.expect === "leak" ? "none" : (m.verifyWith ?? (m.sanitizer !== "none" ? m.sanitizer : "asan_ubsan"));
  const { compile, run } = await compileAndRun(ex.files, { sanitizer, stdin: m.stdin, defines: m.defines });
  if (!compile.ok) return `does not compile:\n${clip(compile.raw)}`;
  const warns = warningsOf(compile);
  if (m.expect === "warning") return warns.length ? null : "expected a compiler warning, got none";
  // Bug demonstrations (ub/leak/race) may legitimately trigger warnings too: only "clean" examples must be warning-free.
  if (warns.length && m.expect === "clean")
    return `compiler warnings (must be clean with -Wall -Wextra -pedantic):\n${warns.join("\n")}`;
  if (!run) return "no run";
  if (run.timedOut) return "timed out";
  switch (m.expect) {
    case "clean": {
      if (run.sanitizerReport) return `sanitizer report: ${run.sanitizerReport.summary}\n${clip(run.stderr)}`;
      if (run.leak) return `leaks ${run.leak.allocations} allocation(s)`;
      if (run.exitCode !== 0) return `exit code ${run.exitCode} ${run.signal ?? ""}\n${clip(run.stderr)}`;
      if (m.expectedStdout !== undefined && !m.nondeterministic && norm(run.stdout) !== norm(m.expectedStdout))
        return `stdout mismatch\n--- expected\n${m.expectedStdout}\n--- got\n${run.stdout}`;
      return null;
    }
    case "ub":
      return run.sanitizerReport && run.sanitizerReport.kind !== "tsan" ? null : `expected an ASan/UBSan report, got none (exit ${run.exitCode})`;
    case "leak":
      return run.leak ? null : "expected the allocation tracker to report a leak";
    case "race":
      return run.sanitizerReport?.kind === "tsan" ? null : "expected a ThreadSanitizer report";
  }
  return null;
}

// ---------------------------------------------------------------- exercises
async function checkExercise(ch: string, id: string): Promise<string | null | { skip: string }> {
  const ex = await loadExerciseInternals(ch, id);
  const miss = missing(ex.meta.requires);
  if (miss.length) return { skip: `needs ${miss.join(", ")}` };
  const merge = (user: SourceFile[]) => [
    ...ex.starter.filter((f) => !ex.meta.editable.includes(f.name)),
    ...ex.meta.editable.map((n) => user.find((f) => f.name === n) ?? ex.starter.find((f) => f.name === n)!).filter(Boolean),
  ];
  for (const n of ex.meta.editable) {
    if (!ex.starter.some((f) => f.name === n)) return `editable file ${n} missing from starter/`;
    if (!ex.solution.some((f) => f.name === n)) return `editable file ${n} missing from solution/`;
  }
  const grade = (files: SourceFile[]) =>
    ex.meta.mode === "io" ? gradeIo(files, ex.io ?? [], ex.meta.sanitizer) : gradeDoctest(files, ex.tests, ex.meta.sanitizer);
  if (ex.meta.mode === "io" && !ex.io?.length) return "io mode needs io-tests.json";
  if (ex.meta.mode === "doctest" && !ex.tests.length) return "no tests/";

  const sol = await grade(merge(ex.solution));
  if (!sol.compile.ok) return `solution does not compile:\n${clip(sol.compile.raw)}`;
  const warns = warningsOf(sol.compile).filter((w) => !w.startsWith("<hidden>"));
  if (warns.length) return `solution has warnings:\n${warns.join("\n")}`;
  if (!sol.passed) return `solution fails:\n${sol.tests.filter((t) => !t.passed).map((t) => `${t.name}: ${t.message}`).join("\n")}`;

  const st = await grade(merge(ex.starter));
  if (!ex.meta.starterCompiles) return st.compile.ok && st.passed ? "starter marked broken but passes" : null;
  if (!st.compile.ok) return `starter does not compile (mark starterCompiles:false if intended):\n${clip(st.compile.raw)}`;
  if (st.passed) return "starter already passes every test (tests are vacuous?)";
  return null;
}

// ---------------------------------------------------------------- projects
async function checkProject(ch: string, id: string): Promise<string | null | { skip: string }> {
  const p = await loadProject(ch, id);
  const miss = missing(p.meta.requires);
  if (miss.length) return { skip: `needs ${miss.join(", ")}` };
  const merge = (starter: SourceFile[], user: SourceFile[]) => [
    ...starter.filter((f) => !p.meta.editable.includes(f.name)),
    ...p.meta.editable.map((n) => user.find((f) => f.name === n) ?? starter.find((f) => f.name === n)!).filter(Boolean),
  ];
  const problems: string[] = [];
  for (let s = 1; s <= p.steps.length; s++) {
    const st = await loadProjectStepInternals(ch, id, s);
    const tests = st.testSets.flatMap((t) => t.tests);
    if (!tests.length) problems.push(`step ${s}: no tests`);
    if (!st.solution.length) problems.push(`step ${s}: no solution`);
    const r = await gradeDoctest(merge(p.starter, st.solution), tests, p.meta.sanitizer);
    if (!r.compile.ok) problems.push(`step ${s} solution does not compile:\n${clip(r.compile.raw)}`);
    else {
      const w = warningsOf(r.compile).filter((x) => !x.startsWith("<hidden>"));
      if (w.length) problems.push(`step ${s} solution warnings:\n${w.join("\n")}`);
      if (!r.passed) problems.push(`step ${s} solution fails: ${r.tests.filter((t) => !t.passed).map((t) => `${t.name}: ${t.message}`).join("; ")}`);
    }
    if (s === 1) {
      const r0 = await gradeDoctest(p.starter, tests, p.meta.sanitizer);
      if (r0.compile.ok && r0.passed) problems.push("starter already passes step 1");
    }
  }
  return problems.length ? problems.join("\n") : null;
}

// ---------------------------------------------------------------- quizzes
async function checkQuiz(ch: string): Promise<string | null> {
  const quiz = await loadQuiz(ch);
  const problems: string[] = [];
  const ids = new Set<string>();
  for (const q of quiz.questions) {
    if (ids.has(q.id)) problems.push(`duplicate id ${q.id}`);
    ids.add(q.id);
    if (q.answer >= q.choices.length) problems.push(`${q.id}: answer out of range`);
    if (!q.verify) continue;
    if (!q.code) {
      problems.push(`${q.id}: verify without code`);
      continue;
    }
    const files = [{ name: "main.cpp", content: q.code }];
    const { compile, run } = await compileAndRun(files, { sanitizer: "asan_ubsan", stdin: q.verify.stdin });
    if (q.verify.expectCompile !== undefined && compile.ok !== q.verify.expectCompile)
      problems.push(`${q.id}: expected compile=${q.verify.expectCompile}, got ${compile.ok}\n${clip(compile.raw, 300)}`);
    if (q.verify.expectStdout !== undefined) {
      if (!run) problems.push(`${q.id}: did not compile:\n${clip(compile.raw, 300)}`);
      else if (norm(run.stdout) !== norm(q.verify.expectStdout))
        problems.push(`${q.id}: stdout ${JSON.stringify(run.stdout)} != expected ${JSON.stringify(q.verify.expectStdout)}`);
    }
    if (q.verify.expectSanitizer !== undefined) {
      const got = !!run?.sanitizerReport;
      if (got !== q.verify.expectSanitizer) problems.push(`${q.id}: expected sanitizer report=${q.verify.expectSanitizer}, got ${got}`);
    }
  }
  return problems.length ? problems.join("\n") : null;
}

// ---------------------------------------------------------------- visualizer
async function checkViz(id: string): Promise<string | null> {
  const s = await loadViz(id);
  const nLines = s.code.split("\n").length;
  const problems: string[] = [];
  s.steps.forEach((st, i) => {
    if (st.line > nLines) problems.push(`step ${i + 1}: line ${st.line} > ${nLines}`);
    const ids = new Set<string>();
    for (const f of st.stack) for (const v of f.vars) if (v.id) ids.add(v.id);
    for (const b of st.heap) {
      ids.add(b.id);
      for (const c of b.cells) if (c.id) ids.add(c.id);
    }
    const refs = [...st.stack.flatMap((f) => f.vars), ...st.heap.flatMap((b) => b.cells)].filter((v) => v.ref);
    for (const v of refs) if (!ids.has(v.ref!)) problems.push(`step ${i + 1}: ${v.name} refs unknown "${v.ref}"`);
  });
  return problems.length ? problems.join("\n") : null;
}

// ---------------------------------------------------------------- lessons
async function checkLessons(ch: string, examples: string[], vizIds: Set<string>): Promise<string | null> {
  const problems: string[] = [];
  const used = new Set<string>();
  for (const l of await listLessons(ch)) {
    const { markdown } = await loadLesson(ch, l.id);
    for (const m of markdown.matchAll(/::example\{id="([^"]+)"\}/g)) {
      const [c, e] = m[1].includes("/") ? m[1].split("/") : [ch, m[1]];
      if (c === ch) used.add(e);
      if (!existsSync(path.join(CONTENT_DIR, c, "examples", e, "meta.json"))) problems.push(`${l.id}: unknown example ${m[1]}`);
    }
    for (const m of markdown.matchAll(/::visualize\{id="([^"]+)"\}/g))
      if (!vizIds.has(m[1])) problems.push(`${l.id}: unknown visualizer scenario ${m[1]}`);
    for (const m of markdown.matchAll(/^::(\w+)/gm))
      if (!["example", "visualize"].includes(m[1]) && !markdown.includes(`:::${m[1]}`)) problems.push(`${l.id}: unknown directive ::${m[1]}`);
  }
  for (const e of examples) if (!used.has(e)) problems.push(`example ${e} is not used by any lesson`);
  return problems.length ? problems.join("\n") : null;
}

// ---------------------------------------------------------------- main
const course = await loadCourse();
const chapters = course.chapters.filter((c) => !chapterFilter.length || chapterFilter.includes(c));
const vizList = await listViz();
const vizIds = new Set(vizList.map((v) => v.id));

for (const ch of chapters) {
  const examples = await listExamples(ch);
  if (want("lessons")) task("lessons", ch, () => checkLessons(ch, examples, vizIds));
  if (want("examples")) for (const e of examples) task("example", `${ch}/${e}`, () => checkExample(ch, e));
  if (want("exercises")) for (const e of await listExercises(ch)) task("exercise", `${ch}/${e.id}`, () => checkExercise(ch, e.id));
  if (want("projects")) for (const p of await listProjects(ch)) task("project", `${ch}/${p.id}`, () => checkProject(ch, p.id));
  if (want("quizzes")) task("quiz", ch, () => checkQuiz(ch));
}
if (want("viz") && !chapterFilter.length) for (const v of vizList) task("viz", v.id, () => checkViz(v.id));

// Sanity check: every directory has the files it should.
for (const ch of chapters) {
  const dir = path.join(CONTENT_DIR, ch);
  for (const sub of ["lessons"]) if (!existsSync(path.join(dir, sub))) console.warn(`⚠ ${ch}: no ${sub}/`);
  if (existsSync(path.join(dir, "examples")))
    for (const e of await readdir(path.join(dir, "examples")))
      if (!existsSync(path.join(dir, "examples", e, "meta.json"))) console.warn(`⚠ ${ch}/examples/${e}: no meta.json`);
}

console.log(`Verifying ${tasks.length} items with ${caps.compiler.version} (concurrency ${CONCURRENCY})…`);
const t0 = Date.now();
let next = 0;
let done = 0;
await Promise.all(
  Array.from({ length: CONCURRENCY }, async () => {
    while (next < tasks.length) {
      const r = await tasks[next++]();
      results.push(r);
      done++;
      const icon = r.status === "pass" ? "✓" : r.status === "skip" ? "–" : "✗";
      process.stdout.write(`${icon} ${r.kind.padEnd(8)} ${r.id}${r.status === "skip" ? `  (skipped: ${r.detail})` : ""}  ${r.ms}ms\n`);
    }
  }),
);

const fails = results.filter((r) => r.status === "fail");
const skips = results.filter((r) => r.status === "skip");
if (fails.length) {
  console.log("\n================ FAILURES ================");
  for (const f of fails) console.log(`\n✗ ${f.kind} ${f.id}\n${f.detail}`);
}
const byKind = new Map<string, { pass: number; fail: number; skip: number }>();
for (const r of results) {
  const k = byKind.get(r.kind) ?? { pass: 0, fail: 0, skip: 0 };
  k[r.status]++;
  byKind.set(r.kind, k);
}
console.log("\n kind      pass  fail  skip");
for (const [k, v] of byKind) console.log(` ${k.padEnd(9)} ${String(v.pass).padStart(4)}  ${String(v.fail).padStart(4)}  ${String(v.skip).padStart(4)}`);
console.log(`\n${results.length - fails.length - skips.length} passed, ${fails.length} failed, ${skips.length} skipped in ${((Date.now() - t0) / 1000).toFixed(1)}s`);
process.exit(fails.length ? 1 : 0);
