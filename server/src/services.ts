import type { GradeResponse, RunRequest, RunResponse, SourceFile } from "../../shared/api.js";
import { build, runBinary } from "./compiler/pipeline.js";
import { withTempDir } from "./compiler/sandbox.js";
import { HttpError, loadExerciseInternals, loadProjectStepInternals } from "./content/loader.js";
import { gradeDoctest, gradeIo } from "./grader/grade.js";

export async function runProgram(req: RunRequest): Promise<RunResponse> {
  if (!Array.isArray(req.files) || req.files.length === 0) throw new HttpError(400, "No files");
  if (req.files.length > 20) throw new HttpError(400, "Too many files");
  const total = req.files.reduce((n, f) => n + (f.content?.length ?? 0), 0);
  if (total > 512 * 1024) throw new HttpError(400, "Source too large");
  return withTempDir(async (dir) => {
    const { compile, binary } = await build(dir, req.files, {
      sanitizer: req.sanitizer ?? "none",
      trackAllocs: !!req.trackAllocs,
      defines: req.defines,
    });
    if (!compile.ok || !binary) return { compile };
    const run = await runBinary(dir, binary, { stdin: req.stdin });
    return { compile, run };
  });
}

/** Learner files only override editable files; read-only starter files come from the server. */
function mergeFiles(starter: SourceFile[], editable: string[], submitted: SourceFile[]): SourceFile[] {
  const out: SourceFile[] = starter.filter((f) => !editable.includes(f.name));
  for (const name of editable) {
    const f = submitted.find((s) => s.name === name) ?? starter.find((s) => s.name === name);
    if (f) out.push({ name, content: f.content });
  }
  return out;
}

export async function gradeExercise(ch: string, id: string, files: SourceFile[]): Promise<GradeResponse> {
  const ex = await loadExerciseInternals(ch, id);
  const merged = mergeFiles(ex.starter, ex.meta.editable, files);
  if (ex.meta.mode === "io") {
    if (!ex.io) throw new Error(`${ch}/${id}: io mode but no io-tests.json`);
    return gradeIo(merged, ex.io, ex.meta.sanitizer);
  }
  return gradeDoctest(merged, ex.tests, ex.meta.sanitizer);
}

export async function gradeProjectStep(ch: string, id: string, step: number, files: SourceFile[]): Promise<GradeResponse> {
  const p = await loadProjectStepInternals(ch, id, step);
  const merged = mergeFiles(p.starter, p.meta.editable, files);
  const tests = p.testSets.flatMap((s) => s.tests);
  return gradeDoctest(merged, tests, p.meta.sanitizer);
}
