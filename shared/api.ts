// Types shared by the server, the web app and the verify script.

export type SanitizerMode = "none" | "asan_ubsan" | "tsan";

export interface SourceFile {
  name: string; // e.g. "main.cpp", "Complexe.hpp"
  content: string;
}

export interface Diagnostic {
  file: string;
  line: number;
  col: number;
  severity: "error" | "warning" | "note";
  message: string;
  flag?: string; // e.g. "-Wunused-variable"
}

export interface CompileResult {
  ok: boolean;
  diagnostics: Diagnostic[];
  raw: string; // full compiler output (harness lines relabelled)
  durationMs: number;
  timedOut?: boolean;
}

export interface SanitizerReport {
  kind: "asan" | "ubsan" | "tsan";
  summary: string;
}

export interface RunOutput {
  stdout: string;
  stderr: string;
  exitCode: number | null;
  signal: string | null;
  timedOut: boolean;
  truncated: boolean;
  durationMs: number;
  sanitizerReport?: SanitizerReport;
  leak?: { allocations: number };
}

export interface RunRequest {
  files: SourceFile[];
  stdin?: string;
  sanitizer?: SanitizerMode;
  trackAllocs?: boolean;
  defines?: string[]; // e.g. ["DEBUG"] -> -DDEBUG
}

export interface RunResponse {
  compile: CompileResult;
  run?: RunOutput;
}

export interface TestResult {
  name: string;
  passed: boolean;
  message?: string;
}

export interface GradeRequest {
  kind: "exercise" | "project";
  id: string; // "<chapter>/<exercise-or-project-id>"
  step?: number; // project step (1-based)
  files: SourceFile[];
}

export interface GradeResponse {
  compile: CompileResult;
  run?: RunOutput;
  tests: TestResult[];
  passed: boolean;
  summary: string;
}

export interface Capabilities {
  compiler: { path: string; version: string; family: "clang" | "gcc" | "unknown" } | null;
  error?: string;
  asanUbsan: boolean;
  tsan: boolean;
  par: { available: boolean; extraFlags: string[] };
  tbb: { available: boolean; extraFlags: string[]; linkFlags: string[] };
}
