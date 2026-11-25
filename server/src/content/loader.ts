import { existsSync } from "node:fs";
import { readdir, readFile } from "node:fs/promises";
import path from "node:path";
import matter from "gray-matter";
import type { SourceFile } from "../../../shared/api.js";
import { REPO_ROOT } from "../compiler/pipeline.js";
import {
  chapterSchema,
  courseSchema,
  drillSchema,
  exampleMeta,
  exerciseMeta,
  ioTests,
  lessonFrontmatter,
  projectMeta,
  quizSchema,
  vizScenario,
} from "./schema.js";

export const CONTENT_DIR = process.env.CPPLAB_CONTENT_DIR ?? path.join(REPO_ROOT, "content");

const ID_RE = /^[a-z0-9][a-z0-9-]*$/;
export function safeId(id: string): string {
  if (!ID_RE.test(id)) throw new HttpError(400, `Invalid id "${id}"`);
  return id;
}

export class HttpError extends Error {
  constructor(
    public status: number,
    message: string,
  ) {
    super(message);
  }
}

async function readJson<T>(file: string, schema: { parse: (x: unknown) => T }): Promise<T> {
  if (!existsSync(file)) throw new HttpError(404, `Not found: ${path.relative(CONTENT_DIR, file)}`);
  const raw = JSON.parse(await readFile(file, "utf8"));
  try {
    return schema.parse(raw);
  } catch (e) {
    throw new Error(`Invalid ${path.relative(CONTENT_DIR, file)}: ${e}`);
  }
}

async function listDirs(dir: string): Promise<string[]> {
  if (!existsSync(dir)) return [];
  return (await readdir(dir, { withFileTypes: true }))
    .filter((d) => d.isDirectory())
    .map((d) => d.name)
    .sort();
}

/** Read all source files in a directory (non-recursive), main.cpp first. */
export async function readSources(dir: string, order?: string[]): Promise<SourceFile[]> {
  if (!existsSync(dir)) return [];
  const names = (await readdir(dir)).filter((n) => /\.(cpp|cc|h|hpp)$/.test(n));
  const rank = (n: string) => {
    const i = order?.indexOf(n) ?? -1;
    if (i >= 0) return i;
    if (n === "main.cpp") return -1;
    return 1000;
  };
  names.sort((a, b) => rank(a) - rank(b) || a.localeCompare(b));
  return Promise.all(names.map(async (name) => ({ name, content: await readFile(path.join(dir, name), "utf8") })));
}

/** Strip the leading number used for ordering: "01-intro" -> "intro"? We keep full ids for stability. */
export const chapterDir = (ch: string) => path.join(CONTENT_DIR, safeId(ch));

export async function loadCourse() {
  return readJson(path.join(CONTENT_DIR, "course.json"), courseSchema);
}

export async function loadChapter(ch: string) {
  return readJson(path.join(chapterDir(ch), "chapter.json"), chapterSchema);
}

export async function listLessons(ch: string) {
  const dir = path.join(chapterDir(ch), "lessons");
  if (!existsSync(dir)) return [];
  const files = (await readdir(dir)).filter((f) => f.endsWith(".md")).sort();
  return Promise.all(
    files.map(async (f) => {
      const { data } = matter(await readFile(path.join(dir, f), "utf8"));
      const fm = lessonFrontmatter.parse(data);
      return { id: f.replace(/\.md$/, ""), title: fm.title, slides: fm.slides };
    }),
  );
}

export async function loadLesson(ch: string, id: string) {
  const file = path.join(chapterDir(ch), "lessons", `${safeId(id)}.md`);
  if (!existsSync(file)) throw new HttpError(404, `Lesson ${ch}/${id} not found`);
  const { data, content } = matter(await readFile(file, "utf8"));
  const fm = lessonFrontmatter.parse(data);
  return { id, ...fm, markdown: content };
}

export async function loadExample(ch: string, id: string) {
  const dir = path.join(chapterDir(ch), "examples", safeId(id));
  const meta = await readJson(path.join(dir, "meta.json"), exampleMeta);
  const files = await readSources(dir, meta.files);
  return { id, chapter: ch, meta, files };
}

export async function listExamples(ch: string) {
  return listDirs(path.join(chapterDir(ch), "examples"));
}

export async function listExercises(ch: string) {
  const ids = await listDirs(path.join(chapterDir(ch), "exercises"));
  return Promise.all(
    ids.map(async (id) => {
      const meta = await readJson(path.join(chapterDir(ch), "exercises", id, "exercise.json"), exerciseMeta);
      return { id, title: meta.title, difficulty: meta.difficulty, requires: meta.requires ?? [] };
    }),
  );
}

export async function loadExercise(ch: string, id: string) {
  const dir = path.join(chapterDir(ch), "exercises", safeId(id));
  const meta = await readJson(path.join(dir, "exercise.json"), exerciseMeta);
  const statement = await readFile(path.join(dir, "statement.md"), "utf8");
  const starter = await readSources(path.join(dir, "starter"), meta.editable);
  return { id, chapter: ch, meta, statement, starter, dir };
}

/** Server-side only: hidden test files + solution. */
export async function loadExerciseInternals(ch: string, id: string) {
  const ex = await loadExercise(ch, id);
  const tests = await readSources(path.join(ex.dir, "tests"));
  const io = existsSync(path.join(ex.dir, "io-tests.json"))
    ? await readJson(path.join(ex.dir, "io-tests.json"), ioTests)
    : undefined;
  const solution = await readSources(path.join(ex.dir, "solution"), ex.meta.editable);
  return { ...ex, tests, io, solution };
}

export async function listProjects(ch: string) {
  const ids = await listDirs(path.join(chapterDir(ch), "projects"));
  return Promise.all(
    ids.map(async (id) => {
      const meta = await readJson(path.join(chapterDir(ch), "projects", id, "project.json"), projectMeta);
      return { id, title: meta.title, steps: meta.steps.length, requires: meta.requires ?? [] };
    }),
  );
}

export async function loadProject(ch: string, id: string) {
  const dir = path.join(chapterDir(ch), "projects", safeId(id));
  const meta = await readJson(path.join(dir, "project.json"), projectMeta);
  const starter = await readSources(path.join(dir, "starter"), meta.editable);
  const steps = await Promise.all(
    meta.steps.map(async (s, i) => ({
      ...s,
      number: i + 1,
      statement: await readFile(path.join(dir, "steps", String(i + 1), "statement.md"), "utf8"),
      /** This step's own test files (shown to the learner). */
      tests: await readSources(path.join(dir, "steps", String(i + 1), "tests")),
    })),
  );
  return { id, chapter: ch, meta, starter, steps, dir };
}

export async function loadProjectStepInternals(ch: string, id: string, step: number) {
  const p = await loadProject(ch, id);
  if (!Number.isInteger(step) || step < 1 || step > p.steps.length) throw new HttpError(400, `Bad step ${step}`);
  const testSets: { step: number; tests: SourceFile[] }[] = [];
  const first = p.steps[step - 1].cumulative ? 1 : step;
  for (let s = first; s <= step; s++) {
    const tests = await readSources(path.join(p.dir, "steps", String(s), "tests"));
    // Prefix with the step number so files from different steps never collide.
    testSets.push({ step: s, tests: tests.map((t) => ({ ...t, name: `step${s}_${t.name}` })) });
  }
  const solution = await readSources(path.join(p.dir, "steps", String(step), "solution"), p.meta.editable);
  return { ...p, testSets, solution };
}

export async function loadQuiz(ch: string) {
  const file = path.join(chapterDir(ch), "quiz.json");
  if (!existsSync(file)) return { questions: [] };
  return readJson(file, quizSchema);
}

export async function loadDrill(ch: string) {
  const file = path.join(chapterDir(ch), "drill.json");
  if (!existsSync(file)) throw new HttpError(404, `No drill for ${ch}`);
  return readJson(file, drillSchema);
}
async function drillSummary(ch: string) {
  if (!existsSync(path.join(chapterDir(ch), "drill.json"))) return null;
  const d = await loadDrill(ch);
  return { title: d.title, count: d.sections.reduce((n, s) => n + s.cards.length, 0) };
}

const VIZ_DIR = () => path.join(CONTENT_DIR, "visualizer");
export async function listViz() {
  if (!existsSync(VIZ_DIR())) return [];
  const files = (await readdir(VIZ_DIR())).filter((f) => f.endsWith(".json")).sort();
  return Promise.all(
    files.map(async (f) => {
      const s = await readJson(path.join(VIZ_DIR(), f), vizScenario);
      return { id: f.replace(/\.json$/, ""), title: s.title, chapter: s.chapter, summary: s.summary, steps: s.steps.length };
    }),
  );
}
export async function loadViz(id: string) {
  const s = await readJson(path.join(VIZ_DIR(), `${safeId(id)}.json`), vizScenario);
  return { id, ...s };
}

/** Course overview for navigation and the dashboard. */
export async function loadOverview() {
  const course = await loadCourse();
  const chapters = await Promise.all(
    course.chapters.map(async (id) => {
      const [chapter, lessons, exercises, projects, quiz, drill] = await Promise.all([
        loadChapter(id),
        listLessons(id),
        listExercises(id),
        listProjects(id),
        loadQuiz(id),
        drillSummary(id),
      ]);
      return { id, ...chapter, lessons, exercises, projects, quizCount: quiz.questions.length, drill };
    }),
  );
  return { ...course, chapters };
}
