import { useCallback, useEffect, useState } from "react";
import type { Diagnostic } from "../../../shared/api";
import { api, type Example, type RunResponse, type SourceFile } from "../lib/api";
import { CAP_HELP, missingCaps, useCaps } from "../lib/caps";
import { useAsync } from "../lib/useAsync";
import { CodeEditor } from "./CodeEditor";
import { Badge, OutputPanel } from "./OutputPanel";
import { RunControls, type RunSettings } from "./RunControls";

const EXPECT_LABEL: Record<string, { text: string; tone: "warn" | "bad" | "info" }> = {
  "compile-error": { text: "does not compile — on purpose", tone: "warn" },
  ub: { text: "contains a bug: undefined behaviour", tone: "bad" },
  leak: { text: "contains a bug: memory leak", tone: "bad" },
  race: { text: "contains a bug: data race", tone: "bad" },
  warning: { text: "triggers a warning — on purpose", tone: "warn" },
};

export function ExampleRunner({ chapter, id }: { chapter: string; id: string }) {
  const { data, error } = useAsync(() => api.example(chapter, id), [chapter, id]);
  if (error) return <div className="my-4 rounded-md border border-red-400 p-3 text-sm text-red-600">Example {chapter}/{id}: {error}</div>;
  if (!data) return <div className="my-4 h-40 animate-pulse rounded-lg bg-zinc-100 dark:bg-zinc-900" />;
  return <Runner example={data} />;
}

function Runner({ example }: { example: Example }) {
  const { meta } = example;
  const { caps } = useCaps();
  const [files, setFiles] = useState<SourceFile[]>(example.files);
  const [settings, setSettings] = useState<RunSettings>({
    sanitizer: meta.sanitizer,
    trackAllocs: meta.trackAllocs,
    stdin: meta.stdin ?? "",
    showStdin: !!meta.stdin,
  });
  const [result, setResult] = useState<RunResponse>();
  const [runErr, setRunErr] = useState<string>();
  const [running, setRunning] = useState(false);
  const [jump, setJump] = useState<{ file: string; line: number; nonce: number }>();
  const missing = missingCaps(caps, meta.requires);
  const edited = files.some((f, i) => f.content !== example.files[i]?.content);

  const run = useCallback(async () => {
    setRunning(true);
    setRunErr(undefined);
    try {
      setResult(
        await api.run({
          files,
          stdin: settings.stdin,
          sanitizer: settings.sanitizer,
          trackAllocs: settings.trackAllocs,
          defines: meta.defines,
        }),
      );
    } catch (e) {
      setRunErr(e instanceof Error ? e.message : String(e));
    } finally {
      setRunning(false);
    }
  }, [files, settings, meta.defines]);

  useEffect(() => {
    setFiles(example.files);
  }, [example]);

  const onKey = (e: React.KeyboardEvent) => {
    if ((e.metaKey || e.ctrlKey) && e.key === "Enter") {
      e.preventDefault();
      if (!running) run();
    }
  };
  const jumpTo = (d: Diagnostic) => setJump({ file: d.file, line: d.line, nonce: Date.now() });
  const label = EXPECT_LABEL[meta.expect];

  return (
    <div className="not-prose my-6 space-y-2" onKeyDown={onKey}>
      <div className="flex flex-wrap items-center gap-2">
        <span className="text-sm font-semibold">▸ {meta.title}</span>
        {meta.courseVersion && <Badge tone="info">as on the slides</Badge>}
        {label && <Badge tone={label.tone}>{label.text}</Badge>}
        {meta.defines?.length ? <Badge tone="info">-D{meta.defines.join(" -D")}</Badge> : null}
      </div>
      {missing.length > 0 && (
        <div className="rounded-md border border-amber-400 bg-amber-50 p-2 text-sm text-amber-900 dark:bg-amber-950/40 dark:text-amber-200">
          {missing.map((m) => (
            <div key={m}>⚠️ {CAP_HELP[m]}</div>
          ))}
        </div>
      )}
      <CodeEditor files={files} onChange={setFiles} diagnostics={result?.compile.diagnostics} jumpTo={jump} />
      <RunControls
        settings={settings}
        onChange={setSettings}
        onRun={run}
        running={running}
        extra={
          edited && (
            <button
              onClick={() => {
                setFiles(example.files);
                setResult(undefined);
              }}
              className="rounded-md px-2 py-1.5 text-zinc-500 hover:text-zinc-800 dark:hover:text-zinc-200"
            >
              ↺ Reset code
            </button>
          )
        }
      />
      {runErr && <div className="text-sm text-red-600">{runErr}</div>}
      <OutputPanel compile={result?.compile} run={result?.run} onJump={jumpTo} />
      {meta.tryThis && meta.tryThis.length > 0 && (
        <details className="rounded-md border border-sky-200 bg-sky-50/60 px-3 py-2 text-sm dark:border-sky-900 dark:bg-sky-950/30">
          <summary className="cursor-pointer font-medium text-sky-800 dark:text-sky-300">🧪 Try this ({meta.tryThis.length})</summary>
          <ul className="mt-2 list-disc space-y-1 pl-5">
            {meta.tryThis.map((t, i) => (
              <li key={i} dangerouslySetInnerHTML={{ __html: inlineMd(t) }} />
            ))}
          </ul>
        </details>
      )}
    </div>
  );
}

/** Minimal inline markdown for short strings: `code`, **bold**, *italic*. Escapes HTML first. */
export function inlineMd(s: string): string {
  return s
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/`([^`]+)`/g, '<code class="rounded bg-black/5 px-1 font-mono text-[0.85em] dark:bg-white/10">$1</code>')
    .replace(/\*\*([^*]+)\*\*/g, "<b>$1</b>")
    .replace(/\*([^*]+)\*/g, "<i>$1</i>");
}
