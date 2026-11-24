import { useEffect, useRef, useState } from "react";
import { Link, useParams } from "react-router-dom";
import type { Diagnostic } from "../../../shared/api";
import { CodeEditor } from "../components/CodeEditor";
import { inlineMd } from "../components/ExampleRunner";
import { Markdown } from "../components/Markdown";
import { Badge, OutputPanel } from "../components/OutputPanel";
import { RunControls, type RunSettings } from "../components/RunControls";
import { TestResults } from "../components/TestResults";
import { api, type Exercise, type GradeResponse, type RunResponse, type SourceFile } from "../lib/api";
import { CAP_HELP, missingCaps, useCaps } from "../lib/caps";
import { useCourse } from "../lib/course";
import { useProgress } from "../lib/progress";
import { useAsync } from "../lib/useAsync";
import { useTrackLast } from "../lib/useLast";

export function Difficulty({ n }: { n: number }) {
  return (
    <span className="font-mono text-xs tracking-tighter text-amber-500" title={`difficulty ${n}/5`}>
      {"★".repeat(n)}
      <span className="text-zinc-300 dark:text-zinc-700">{"★".repeat(5 - n)}</span>
    </span>
  );
}

export function ExercisePage() {
  const { ch = "", id = "" } = useParams();
  const { data, error } = useAsync(() => api.exercise(ch, id), [ch, id]);
  if (error) return <div className="text-red-600">{error}</div>;
  if (!data) return <div className="h-64 animate-pulse rounded-xl bg-zinc-100 dark:bg-zinc-900" />;
  return <ExerciseView key={`${ch}/${id}`} ex={data} />;
}

function ExerciseView({ ex }: { ex: Exercise }) {
  const key = `${ex.chapter}/${ex.id}`;
  const { progress, update, loaded } = useProgress();
  const { course } = useCourse();
  const { caps } = useCaps();
  const saved = progress.exercises[key];
  const [files, setFiles] = useState<SourceFile[]>(ex.starter);
  const [grade, setGrade] = useState<GradeResponse>();
  const [runRes, setRunRes] = useState<RunResponse>();
  const [busy, setBusy] = useState<"grade" | "run" | null>(null);
  const [err, setErr] = useState<string>();
  const [solution, setSolution] = useState<SourceFile[]>();
  const [jump, setJump] = useState<{ file: string; line: number; nonce: number }>();
  const [settings, setSettings] = useState<RunSettings>({ sanitizer: ex.meta.sanitizer, trackAllocs: false, stdin: "", showStdin: ex.meta.mode === "io" });
  const restored = useRef(false);
  useTrackLast(`Exercise: ${ex.meta.title}`);

  // Restore saved code once progress is loaded.
  useEffect(() => {
    if (!loaded || restored.current) return;
    restored.current = true;
    if (saved?.code?.length) {
      setFiles(ex.starter.map((f) => saved.code!.find((s) => s.name === f.name && ex.meta.editable.includes(f.name)) ?? f));
    }
  }, [loaded, saved, ex]);

  const saveTimer = useRef<number | undefined>(undefined);
  const onChange = (fs: SourceFile[]) => {
    setFiles(fs);
    window.clearTimeout(saveTimer.current);
    saveTimer.current = window.setTimeout(() => {
      update((p) => ({
        ...p,
        exercises: {
          ...p.exercises,
          [key]: { ...(p.exercises[key] ?? { passed: false, attempts: 0 }), code: fs.filter((f) => ex.meta.editable.includes(f.name)) },
        },
      }));
    }, 800);
  };

  const submit = async () => {
    setBusy("grade");
    setErr(undefined);
    setRunRes(undefined);
    try {
      const r = await api.grade({ kind: "exercise", id: key, files: files.filter((f) => ex.meta.editable.includes(f.name)) });
      setGrade(r);
      update((p) => {
        const prev = p.exercises[key] ?? { passed: false, attempts: 0 };
        return {
          ...p,
          exercises: {
            ...p.exercises,
            [key]: { ...prev, attempts: prev.attempts + 1, passed: prev.passed || r.passed, code: files.filter((f) => ex.meta.editable.includes(f.name)) },
          },
        };
      });
    } catch (e) {
      setErr(e instanceof Error ? e.message : String(e));
    } finally {
      setBusy(null);
    }
  };

  const run = async () => {
    setBusy("run");
    setErr(undefined);
    try {
      setRunRes(await api.run({ files, stdin: settings.stdin, sanitizer: settings.sanitizer, trackAllocs: settings.trackAllocs }));
    } catch (e) {
      setErr(e instanceof Error ? e.message : String(e));
    } finally {
      setBusy(null);
    }
  };

  const hintsShown = saved?.hintsShown ?? 0;
  const showHint = () =>
    update((p) => ({ ...p, exercises: { ...p.exercises, [key]: { ...(p.exercises[key] ?? { passed: false, attempts: 0 }), hintsShown: hintsShown + 1 } } }));
  const attempts = saved?.attempts ?? 0;
  const revealSolution = async () => {
    if (!saved?.passed && !confirm("Reveal the reference solution? Try a bit more first if you can.")) return;
    const s = await api.exerciseSolution(ex.chapter, ex.id);
    setSolution(s.files);
    update((p) => ({ ...p, exercises: { ...p.exercises, [key]: { ...(p.exercises[key] ?? { passed: false, attempts: 0 }), solutionShown: true } } }));
  };
  const jumpTo = (d: Diagnostic) => setJump({ file: d.file, line: d.line, nonce: Date.now() });
  const compile = grade?.compile ?? runRes?.compile;
  const missing = missingCaps(caps, ex.meta.requires);

  const chapter = course?.chapters.find((c) => c.id === ex.chapter);
  const idx = chapter?.exercises.findIndex((e) => e.id === ex.id) ?? -1;
  const next = chapter?.exercises[idx + 1];

  return (
    <div className="space-y-5" onKeyDown={(e) => {
      if ((e.metaKey || e.ctrlKey) && e.key === "Enter") { e.preventDefault(); if (!busy) submit(); }
    }}>
      <header>
        <div className="text-sm text-zinc-500">
          <Link to={`/ch/${ex.chapter}`} className="hover:underline">
            Chapter {chapter?.number} · Exercise {idx + 1}/{chapter?.exercises.length}
          </Link>
        </div>
        <div className="flex flex-wrap items-center gap-3">
          <h1 className="text-2xl font-bold tracking-tight">{ex.meta.title}</h1>
          <Difficulty n={ex.meta.difficulty} />
          {saved?.passed && <Badge tone="ok">passed</Badge>}
          <Badge tone="info">{ex.meta.mode === "io" ? "graded on stdin/stdout" : "graded by unit tests"}</Badge>
          {ex.meta.sanitizer !== "none" && <Badge tone="info">{ex.meta.sanitizer === "tsan" ? "TSan" : "ASan + UBSan"} on</Badge>}
        </div>
      </header>
      {missing.length > 0 && (
        <div className="rounded-md border border-amber-400 bg-amber-50 p-2 text-sm text-amber-900 dark:bg-amber-950/40 dark:text-amber-200">
          {missing.map((m) => <div key={m}>⚠️ {CAP_HELP[m]}</div>)}
        </div>
      )}
      <Markdown chapter={ex.chapter}>{ex.statement}</Markdown>
      <CodeEditor
        files={files}
        onChange={onChange}
        readOnlyFiles={ex.starter.map((f) => f.name).filter((n) => !ex.meta.editable.includes(n))}
        diagnostics={compile?.diagnostics}
        jumpTo={jump}
        minLines={14}
      />
      <div className="flex flex-wrap items-center gap-2">
        <button
          onClick={submit}
          disabled={!!busy}
          className="inline-flex items-center gap-1.5 rounded-md bg-[#004482] px-4 py-1.5 text-sm font-medium text-white shadow-sm hover:bg-[#00376a] disabled:opacity-50"
          title="Ctrl/Cmd+Enter"
        >
          {busy === "grade" ? "Checking…" : "✓ Check my solution"}
        </button>
        {ex.meta.hints.length > 0 && (
          <button onClick={showHint} disabled={hintsShown >= ex.meta.hints.length} className="rounded-md border border-zinc-300 px-3 py-1.5 text-sm disabled:opacity-40 dark:border-zinc-700">
            💡 Hint ({hintsShown}/{ex.meta.hints.length})
          </button>
        )}
        <button
          onClick={revealSolution}
          disabled={attempts === 0 && !saved?.passed}
          title={attempts === 0 ? "Submit at least one attempt first" : ""}
          className="rounded-md border border-zinc-300 px-3 py-1.5 text-sm disabled:opacity-40 dark:border-zinc-700"
        >
          🔑 Reference solution
        </button>
        <button
          onClick={() => {
            if (confirm("Reset the code to the starter version?")) onChange(ex.starter);
          }}
          className="rounded-md px-2 py-1.5 text-sm text-zinc-500 hover:text-zinc-800 dark:hover:text-zinc-200"
        >
          ↺ Reset
        </button>
        {saved?.passed && next && (
          <Link to={`/ch/${ex.chapter}/exercise/${next.id}`} className="ml-auto text-sm text-sky-600 hover:underline dark:text-sky-400">
            Next exercise: {next.title} →
          </Link>
        )}
      </div>
      {hintsShown > 0 && (
        <ol className="list-decimal space-y-1 rounded-md border border-amber-300 bg-amber-50 py-2 pl-8 pr-3 text-sm dark:border-amber-900 dark:bg-amber-950/30">
          {ex.meta.hints.slice(0, hintsShown).map((h, i) => (
            <li key={i} dangerouslySetInnerHTML={{ __html: inlineMd(h) }} />
          ))}
        </ol>
      )}
      {err && <div className="text-sm text-red-600">{err}</div>}
      {grade && grade.compile.ok && <TestResults result={grade} />}
      {compile && (!compile.ok || compile.diagnostics.length > 0 || runRes) && (
        <OutputPanel compile={compile} run={runRes?.run ?? (grade?.run?.stderr ? grade.run : undefined)} onJump={jumpTo} hideStdout={!runRes} />
      )}
      {ex.meta.mode === "io" && (
        <details className="rounded-md border border-zinc-200 p-3 dark:border-zinc-800">
          <summary className="cursor-pointer text-sm font-medium">Run it yourself with custom input</summary>
          <div className="mt-2">
            <RunControls settings={settings} onChange={setSettings} onRun={run} running={busy === "run"} />
          </div>
        </details>
      )}
      {solution && (
        <section className="space-y-2">
          <h2 className="font-semibold">Reference solution</h2>
          <CodeEditor files={solution} readOnly />
        </section>
      )}
    </div>
  );
}
