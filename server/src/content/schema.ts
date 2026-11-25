import { z } from "zod";

export const sanitizerMode = z.enum(["none", "asan_ubsan", "tsan"]);
export const capability = z.enum(["par", "tbb", "tsan", "asan_ubsan"]);

export const courseSchema = z.object({
  title: z.string(),
  subtitle: z.string().optional(),
  chapters: z.array(z.string()).min(1),
});

export const chapterSchema = z.object({
  number: z.number().int(),
  title: z.string(),
  slides: z.string(), // e.g. "1–25"
  summary: z.string(),
});

export const lessonFrontmatter = z.object({
  title: z.string(),
  slides: z.string().optional(),
});

export const exampleMeta = z.object({
  title: z.string(),
  /** Display order of the files; defaults to main.cpp first then alphabetical. */
  files: z.array(z.string()).optional(),
  stdin: z.string().optional(),
  expectedStdout: z.string().optional(),
  /** Sanitizer preselected in the UI. */
  sanitizer: sanitizerMode.default("none"),
  trackAllocs: z.boolean().default(false),
  defines: z.array(z.string()).optional(),
  /**
   * What running this example demonstrates:
   *  clean         compiles without warnings, runs clean under sanitizers
   *  compile-error must fail to compile (e.g. multiple definition)
   *  ub            sanitizer (ASan/UBSan) must report an error
   *  leak          the allocation tracker must report a leak
   *  race          ThreadSanitizer must report a data race
   *  warning       compiles but must produce a warning (e.g. -Wvexing-parse)
   */
  expect: z.enum(["clean", "compile-error", "ub", "leak", "race", "warning"]).default("clean"),
  /** Sanitizer used by verify-content for "clean" runs (default asan_ubsan). */
  verifyWith: sanitizerMode.optional(),
  requires: z.array(capability).optional(),
  tryThis: z.array(z.string()).optional(),
  /** Reproduces the slide code as written (possibly with its known issues). */
  courseVersion: z.boolean().default(false),
  /** Do not compare stdout (timing-dependent, random...). */
  nondeterministic: z.boolean().default(false),
});
export type ExampleMeta = z.infer<typeof exampleMeta>;

export const exerciseMeta = z.object({
  title: z.string(),
  difficulty: z.number().int().min(1).max(5),
  mode: z.enum(["doctest", "io"]).default("doctest"),
  sanitizer: sanitizerMode.default("asan_ubsan"),
  /** Files the learner may edit (others in starter/ are shown read-only). */
  editable: z.array(z.string()).min(1),
  hints: z.array(z.string()).default([]),
  requires: z.array(capability).optional(),
  tags: z.array(z.string()).default([]),
  /** Set when the starter is intentionally broken (e.g. missing include guards). */
  starterCompiles: z.boolean().default(true),
});
export type ExerciseMeta = z.infer<typeof exerciseMeta>;

export const ioTests = z.array(
  z.object({
    name: z.string(),
    stdin: z.string().default(""),
    expectedStdout: z.string(),
    match: z.enum(["exact", "trim", "regex"]).default("trim"),
  }),
);
export type IoTests = z.infer<typeof ioTests>;

export const projectMeta = z.object({
  title: z.string(),
  summary: z.string(),
  sanitizer: sanitizerMode.default("asan_ubsan"),
  editable: z.array(z.string()).min(1),
  requires: z.array(capability).optional(),
  steps: z
    .array(
      z.object({
        title: z.string(),
        /** Run the tests of all previous steps too (default true). */
        cumulative: z.boolean().default(true),
        hints: z.array(z.string()).default([]),
      }),
    )
    .min(1),
});
export type ProjectMeta = z.infer<typeof projectMeta>;

export const quizQuestion = z.object({
  id: z.string(),
  kind: z.enum(["output", "compiles", "ub", "which-ctor", "concept"]),
  question: z.string(),
  code: z.string().optional(),
  choices: z.array(z.string()).min(2),
  answer: z.number().int().min(0),
  explanation: z.string().min(1),
  slides: z.string().optional(),
  /** Machine check run by verify-content against `code`. */
  verify: z
    .object({
      expectCompile: z.boolean().optional(),
      expectStdout: z.string().optional(),
      expectSanitizer: z.boolean().optional(),
      stdin: z.string().optional(),
    })
    .optional(),
});
export const quizSchema = z.object({ questions: z.array(quizQuestion).min(1) });
export type QuizQuestion = z.infer<typeof quizQuestion>;

export const drillSchema = z.object({
  title: z.string(),
  intro: z.string(),
  sections: z
    .array(
      z.object({
        title: z.string(),
        /** Short reminder of the rule this section trains (Markdown). */
        rule: z.string(),
        /** Code every card of the section is judged against ("Given:"). */
        prelude: z.string(),
        cards: z
          .array(
            z.object({
              code: z.string(),
              /** "main" (default): the card is a statement inside main(); "global": a definition at file scope. */
              scope: z.enum(["main", "global"]).default("main"),
              compiles: z.boolean(),
              explanation: z.string().min(1),
            }),
          )
          .min(1),
      }),
    )
    .min(1),
});
export type Drill = z.infer<typeof drillSchema>;

const vizVar = z.object({
  name: z.string(),
  type: z.string().optional(),
  value: z.string().optional(),
  /** id of the cell/block this pointer or reference points to */
  ref: z.string().optional(),
  kind: z.enum(["value", "pointer", "reference"]).default("value"),
  id: z.string().optional(), // so that pointers can target this variable
  highlight: z.boolean().optional(),
});
export const vizScenario = z.object({
  title: z.string(),
  chapter: z.string(),
  summary: z.string(),
  code: z.string(),
  steps: z
    .array(
      z.object({
        line: z.number().int().min(1),
        note: z.string(),
        stack: z.array(z.object({ frame: z.string(), vars: z.array(vizVar), dead: z.boolean().optional() })),
        heap: z
          .array(
            z.object({
              id: z.string(),
              type: z.string(),
              cells: z.array(vizVar).default([]),
              state: z.enum(["live", "freed", "leaked"]).default("live"),
              label: z.string().optional(),
            }),
          )
          .default([]),
        output: z.string().optional(),
      }),
    )
    .min(1),
});
export type VizScenario = z.infer<typeof vizScenario>;
