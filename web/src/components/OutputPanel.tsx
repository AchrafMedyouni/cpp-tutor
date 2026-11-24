import { useState } from "react";
import type { CompileResult, Diagnostic, RunOutput } from "../../../shared/api";

export function DiagnosticsList({ diagnostics, onJump }: { diagnostics: Diagnostic[]; onJump?: (d: Diagnostic) => void }) {
  const shown = diagnostics.filter((d) => d.severity !== "note");
  if (!shown.length) return null;
  return (
    <ul className="space-y-1 text-sm">
      {shown.map((d, i) => (
        <li key={i}>
          <button
            onClick={() => onJump?.(d)}
            disabled={d.file.startsWith("<")}
            className="flex w-full items-start gap-2 rounded px-1 text-left hover:bg-zinc-100 dark:hover:bg-zinc-800"
          >
            <span className={`mt-0.5 shrink-0 rounded px-1.5 text-xs font-semibold ${d.severity === "error" ? "bg-red-600 text-white" : "bg-amber-400 text-black"}`}>
              {d.severity}
            </span>
            <span className="shrink-0 font-mono text-xs text-zinc-500 mt-0.5">
              {d.file === "<hidden>" ? "test harness" : d.file === "<linker>" ? "linker" : `${d.file}:${d.line}:${d.col}`}
            </span>
            <span className="font-mono text-xs">
              {d.message}
              {d.flag && <span className="text-zinc-500"> [{d.flag}]</span>}
            </span>
          </button>
        </li>
      ))}
    </ul>
  );
}

export function OutputPanel({
  compile,
  run,
  onJump,
  hideStdout,
}: {
  compile?: CompileResult;
  run?: RunOutput;
  onJump?: (d: Diagnostic) => void;
  hideStdout?: boolean;
}) {
  const [showRaw, setShowRaw] = useState(false);
  if (!compile) return null;
  const warnings = compile.diagnostics.filter((d) => d.severity === "warning").length;
  const errors = compile.diagnostics.filter((d) => d.severity === "error").length;
  return (
    <div className="space-y-2 rounded-lg border border-zinc-200 bg-zinc-50 p-3 dark:border-zinc-800 dark:bg-zinc-900/60">
      <div className="flex flex-wrap items-center gap-2 text-xs">
        <Badge tone={compile.ok ? "ok" : "bad"}>{compile.ok ? "compiled" : "compilation failed"}</Badge>
        {warnings > 0 && <Badge tone="warn">{warnings} warning{warnings > 1 ? "s" : ""}</Badge>}
        {errors > 0 && <Badge tone="bad">{errors} error{errors > 1 ? "s" : ""}</Badge>}
        {run && (
          <>
            <Badge tone={run.exitCode === 0 && !run.signal ? "ok" : "bad"}>
              {run.timedOut ? "timed out" : run.signal ? `killed by ${run.signal}` : `exit code ${run.exitCode}`}
            </Badge>
            <span className="text-zinc-500">{run.durationMs} ms</span>
          </>
        )}
        <span className="text-zinc-500">compile {compile.durationMs} ms</span>
        {compile.raw.trim() && (
          <button className="ml-auto text-sky-600 hover:underline dark:text-sky-400" onClick={() => setShowRaw(!showRaw)}>
            {showRaw ? "hide" : "show"} compiler output
          </button>
        )}
      </div>
      <DiagnosticsList diagnostics={compile.diagnostics} onJump={onJump} />
      {showRaw && <Pre className="text-zinc-600 dark:text-zinc-400">{compile.raw}</Pre>}
      {run?.sanitizerReport && (
        <div className="rounded-md border border-red-300 bg-red-50 p-2 text-sm text-red-800 dark:border-red-800 dark:bg-red-950/40 dark:text-red-300">
          <b>🛑 {run.sanitizerReport.kind.toUpperCase()} report:</b> {run.sanitizerReport.summary}
        </div>
      )}
      {run?.leak && (
        <div className="rounded-md border border-orange-300 bg-orange-50 p-2 text-sm text-orange-800 dark:border-orange-800 dark:bg-orange-950/40 dark:text-orange-300">
          <b>💧 Memory leak:</b> {run.leak.allocations} allocation(s) were never released.
        </div>
      )}
      {run && !hideStdout && (
        <div>
          <div className="mb-1 text-xs font-semibold uppercase tracking-wide text-zinc-500">stdout</div>
          <Pre>{run.stdout || <span className="text-zinc-400">(no output)</span>}</Pre>
        </div>
      )}
      {run?.stderr && (
        <details open={!!run.sanitizerReport || run.stderr.length < 600}>
          <summary className="cursor-pointer text-xs font-semibold uppercase tracking-wide text-zinc-500">stderr</summary>
          <Pre className="text-red-700 dark:text-red-300">{run.stderr}</Pre>
        </details>
      )}
    </div>
  );
}

function Pre({ children, className = "" }: { children: React.ReactNode; className?: string }) {
  return (
    <pre className={`max-h-96 overflow-auto whitespace-pre-wrap rounded-md bg-white p-2 font-mono text-xs dark:bg-zinc-950 ${className}`}>
      {children}
    </pre>
  );
}

export function Badge({ tone, children }: { tone: "ok" | "bad" | "warn" | "info"; children: React.ReactNode }) {
  const cls = {
    ok: "bg-emerald-100 text-emerald-800 dark:bg-emerald-900/50 dark:text-emerald-300",
    bad: "bg-red-100 text-red-800 dark:bg-red-900/50 dark:text-red-300",
    warn: "bg-amber-100 text-amber-900 dark:bg-amber-900/50 dark:text-amber-300",
    info: "bg-sky-100 text-sky-800 dark:bg-sky-900/50 dark:text-sky-300",
  }[tone];
  return <span className={`rounded px-1.5 py-0.5 font-medium ${cls}`}>{children}</span>;
}
