import { useEffect, useState } from "react";
import { Link, useParams } from "react-router-dom";
import { CodeEditor } from "../components/CodeEditor";
import { inlineMd } from "../components/ExampleRunner";
import { api, type VizScenario } from "../lib/api";
import { useCourse } from "../lib/course";
import { useProgress } from "../lib/progress";
import { useAsync } from "../lib/useAsync";
import { useTrackLast } from "../lib/useLast";
import { MemoryView } from "../visualizer/MemoryView";

export function VisualizerList() {
  const { data, error } = useAsync(() => api.vizList(), []);
  const { course } = useCourse();
  const { progress } = useProgress();
  useTrackLast("Memory visualizer");
  if (error) return <div className="text-red-600">{error}</div>;
  return (
    <div className="space-y-6">
      <header>
        <h1 className="text-3xl font-bold tracking-tight">Memory visualizer</h1>
        <p className="mt-2 max-w-3xl text-zinc-600 dark:text-zinc-400">
          Step through small programs line by line and watch the <b>stack frames</b>, the <b>heap blocks</b> and every{" "}
          <span className="text-sky-600">pointer →</span> / <span className="text-violet-500">reference ⇢</span>. Freed blocks are hatched, leaks
          are outlined in red, and dangling pointers turn red.
        </p>
      </header>
      <div className="grid gap-3 md:grid-cols-2">
        {data?.map((s) => {
          const ch = course?.chapters.find((c) => c.id === s.chapter);
          return (
            <Link
              key={s.id}
              to={`/visualizer/${s.id}`}
              className="rounded-xl border border-zinc-200 p-4 hover:border-zinc-400 dark:border-zinc-800 dark:hover:border-zinc-600"
            >
              <div className="flex items-baseline justify-between gap-2">
                <span className="font-semibold">{s.title}</span>
                {progress.vizSeen[s.id] && <span className="text-emerald-600">✓</span>}
              </div>
              <div className="mt-1 text-sm text-zinc-600 dark:text-zinc-400">{s.summary}</div>
              <div className="mt-2 text-xs text-zinc-500">
                {ch ? `Chapter ${ch.number} · ` : ""}
                {s.steps} steps
              </div>
            </Link>
          );
        })}
      </div>
    </div>
  );
}

export function VisualizerPage() {
  const { id = "" } = useParams();
  const { data, error } = useAsync(() => api.viz(id), [id]);
  if (error) return <div className="text-red-600">{error}</div>;
  if (!data) return <div className="h-64 animate-pulse rounded-xl bg-zinc-100 dark:bg-zinc-900" />;
  return <Player key={id} s={data} />;
}

function Player({ s }: { s: VizScenario }) {
  const [i, setI] = useState(0);
  const { update } = useProgress();
  const { course } = useCourse();
  useTrackLast(`Visualizer: ${s.title}`);
  const step = s.steps[i];
  const last = s.steps.length - 1;
  const ch = course?.chapters.find((c) => c.id === s.chapter);

  useEffect(() => {
    if (i === last) update((p) => ({ ...p, vizSeen: { ...p.vizSeen, [s.id]: true } }));
  }, [i, last, s.id, update]);

  useEffect(() => {
    const onKey = (e: KeyboardEvent) => {
      if (e.target instanceof HTMLElement && (e.target.closest(".monaco-editor") || e.target.tagName === "INPUT")) return;
      if (e.key === "ArrowRight") setI((x) => Math.min(x + 1, last));
      if (e.key === "ArrowLeft") setI((x) => Math.max(x - 1, 0));
    };
    window.addEventListener("keydown", onKey);
    return () => window.removeEventListener("keydown", onKey);
  }, [last]);

  return (
    <div className="space-y-4">
      <header>
        <div className="text-sm text-zinc-500">
          <Link to="/visualizer" className="hover:underline">Memory visualizer</Link>
          {ch && (
            <>
              {" · "}
              <Link to={`/ch/${ch.id}`} className="hover:underline">Chapter {ch.number}</Link>
            </>
          )}
        </div>
        <h1 className="text-2xl font-bold tracking-tight">{s.title}</h1>
        <p className="text-zinc-600 dark:text-zinc-400">{s.summary}</p>
      </header>

      <CodeEditor files={[{ name: "main.cpp", content: s.code }]} readOnly activeLine={step.line} minLines={6} maxLines={18} />

      <div className="rounded-lg border border-sky-200 bg-sky-50 px-4 py-3 text-sm dark:border-sky-900 dark:bg-sky-950/30">
        <span className="mr-2 font-mono text-xs text-sky-700 dark:text-sky-300">line {step.line}</span>
        <span dangerouslySetInnerHTML={{ __html: inlineMd(step.note) }} />
        {step.output !== undefined && (
          <div className="mt-2">
            <span className="text-xs font-semibold uppercase text-zinc-500">stdout so far</span>
            <pre className="mt-1 rounded bg-white p-2 font-mono text-xs dark:bg-zinc-950">{step.output || " "}</pre>
          </div>
        )}
      </div>

      <div className="flex items-center gap-3">
        <button onClick={() => setI(0)} disabled={i === 0} className="rounded-md border border-zinc-300 px-2 py-1.5 text-sm disabled:opacity-40 dark:border-zinc-700">⏮</button>
        <button onClick={() => setI(Math.max(i - 1, 0))} disabled={i === 0} className="rounded-md border border-zinc-300 px-3 py-1.5 text-sm disabled:opacity-40 dark:border-zinc-700">← Previous</button>
        <button onClick={() => setI(Math.min(i + 1, last))} disabled={i === last} className="rounded-md bg-[#004482] px-4 py-1.5 text-sm font-medium text-white disabled:opacity-40">Next →</button>
        <input type="range" min={0} max={last} value={i} onChange={(e) => setI(Number(e.target.value))} className="flex-1" aria-label="step" />
        <span className="w-20 text-right font-mono text-sm text-zinc-500">{i + 1}/{s.steps.length}</span>
      </div>
      <div className="rounded-lg border border-zinc-200 p-3 dark:border-zinc-800">
        <MemoryView step={step} />
      </div>
      <p className="text-xs text-zinc-500">Tip: use the ← and → keys.</p>
    </div>
  );
}
