import type {
  Capabilities,
  GradeRequest,
  GradeResponse,
  RunRequest,
  RunResponse,
  SanitizerMode,
  SourceFile,
} from "../../../shared/api";

export type { Capabilities, GradeResponse, RunResponse, SanitizerMode, SourceFile };

async function req<T>(url: string, init?: RequestInit): Promise<T> {
  const r = await fetch(url, { headers: { "Content-Type": "application/json" }, ...init });
  if (!r.ok) {
    let msg = `${r.status} ${r.statusText}`;
    try {
      msg = (await r.json()).error ?? msg;
    } catch {
      /* ignore */
    }
    throw new Error(msg);
  }
  return r.json() as Promise<T>;
}
const get = <T,>(url: string) => req<T>(url);
const post = <T,>(url: string, body: unknown) => req<T>(url, { method: "POST", body: JSON.stringify(body) });

export type Capability = "par" | "tbb" | "tsan" | "asan_ubsan";

export interface LessonRef { id: string; title: string; slides?: string }
export interface ExerciseRef { id: string; title: string; difficulty: number; requires: Capability[] }
export interface ProjectRef { id: string; title: string; steps: number; requires: Capability[] }
export interface ChapterOverview {
  id: string;
  number: number;
  title: string;
  slides: string;
  summary: string;
  lessons: LessonRef[];
  exercises: ExerciseRef[];
  projects: ProjectRef[];
  quizCount: number;
  drill: { title: string; count: number } | null;
}
export interface Overview { title: string; subtitle?: string; chapters: ChapterOverview[] }

export interface Lesson { id: string; title: string; slides?: string; markdown: string }

export interface ExampleMeta {
  title: string;
  stdin?: string;
  expectedStdout?: string;
  sanitizer: SanitizerMode;
  trackAllocs: boolean;
  defines?: string[];
  expect: "clean" | "compile-error" | "ub" | "leak" | "race" | "warning";
  requires?: Capability[];
  tryThis?: string[];
  courseVersion: boolean;
}
export interface Example { id: string; chapter: string; meta: ExampleMeta; files: SourceFile[] }

export interface Exercise {
  id: string;
  chapter: string;
  meta: {
    title: string;
    difficulty: number;
    mode: "doctest" | "io";
    sanitizer: SanitizerMode;
    editable: string[];
    hints: string[];
    requires?: Capability[];
    tags: string[];
  };
  statement: string;
  starter: SourceFile[];
  /** Test files the solution is graded against (doctest mode). */
  tests: SourceFile[];
  /** stdin/stdout cases (io mode). */
  ioTests?: IoTest[];
}
export interface IoTest { name: string; stdin: string; expectedStdout: string; match: "exact" | "trim" | "regex" }

export interface Project {
  id: string;
  chapter: string;
  meta: { title: string; summary: string; sanitizer: SanitizerMode; editable: string[]; requires?: Capability[] };
  starter: SourceFile[];
  steps: { number: number; title: string; statement: string; hints: string[]; cumulative: boolean; tests: SourceFile[] }[];
}

export interface QuizQuestion {
  id: string;
  kind: "output" | "compiles" | "ub" | "which-ctor" | "concept";
  question: string;
  code?: string;
  choices: string[];
  answer: number;
  explanation: string;
  slides?: string;
}

export interface DrillCard { code: string; scope: "main" | "global"; compiles: boolean; explanation: string }
export interface DrillSection { title: string; rule: string; prelude: string; cards: DrillCard[] }
export interface Drill { title: string; intro: string; sections: DrillSection[] }

export interface VizVar {
  name: string;
  type?: string;
  value?: string;
  ref?: string;
  kind: "value" | "pointer" | "reference";
  id?: string;
  highlight?: boolean;
}
export interface VizStep {
  line: number;
  note: string;
  stack: { frame: string; vars: VizVar[]; dead?: boolean }[];
  heap: { id: string; type: string; cells: VizVar[]; state: "live" | "freed" | "leaked"; label?: string }[];
  output?: string;
}
export interface VizScenario { id: string; title: string; chapter: string; summary: string; code: string; steps: VizStep[] }
export interface VizRef { id: string; title: string; chapter: string; summary: string; steps: number }

export const api = {
  health: () => get<{ ok: boolean; capabilities: Capabilities }>("/api/health"),
  course: () => get<Overview>("/api/course"),
  lesson: (ch: string, id: string) => get<Lesson>(`/api/chapters/${ch}/lessons/${id}`),
  example: (ch: string, id: string) => get<Example>(`/api/chapters/${ch}/examples/${id}`),
  quiz: (ch: string) => get<{ questions: QuizQuestion[] }>(`/api/chapters/${ch}/quiz`),
  drill: (ch: string) => get<Drill>(`/api/chapters/${ch}/drill`),
  exercise: (ch: string, id: string) => get<Exercise>(`/api/chapters/${ch}/exercises/${id}`),
  exerciseSolution: (ch: string, id: string) => get<{ files: SourceFile[] }>(`/api/chapters/${ch}/exercises/${id}/solution`),
  project: (ch: string, id: string) => get<Project>(`/api/chapters/${ch}/projects/${id}`),
  projectSolution: (ch: string, id: string, step: number) =>
    get<{ files: SourceFile[] }>(`/api/chapters/${ch}/projects/${id}/steps/${step}/solution`),
  vizList: () => get<VizRef[]>("/api/visualizer"),
  viz: (id: string) => get<VizScenario>(`/api/visualizer/${id}`),
  run: (body: RunRequest) => post<RunResponse>("/api/run", body),
  grade: (body: GradeRequest) => post<GradeResponse>("/api/grade", body),
  getProgress: () => get<unknown>("/api/progress"),
  putProgress: (p: unknown) => req<{ ok: boolean }>("/api/progress", { method: "PUT", body: JSON.stringify(p) }),
};
