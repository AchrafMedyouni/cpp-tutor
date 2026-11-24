import { useEffect, useRef, useState } from "react";
import { Link, useParams } from "react-router-dom";
import type { Diagnostic } from "../../../shared/api";
import { CodeEditor } from "../components/CodeEditor";
import { inlineMd } from "../components/ExampleRunner";
import { Markdown } from "../components/Markdown";
import { Badge, OutputPanel } from "../components/OutputPanel";
import { TestResults } from "../components/TestResults";
import { api, type GradeResponse, type Project, type SourceFile } from "../lib/api";
import { CAP_HELP, missingCaps, useCaps } from "../lib/caps";
import { useCourse } from "../lib/course";
import { useProgress } from "../lib/progress";
import { useAsync } from "../lib/useAsync";
import { useTrackLast } from "../lib/useLast";

export function ProjectPage() {
  const { ch = "", id = "" } = useParams();
  const { data, error } = useAsync(() => api.project(ch, id), [ch, id]);
  if (error) return <div className="text-red-600">{error}</div>;
  if (!data) return <div className="h-64 animate-pulse rounded-xl bg-zinc-100 dark:bg-zinc-900" />;
  return <ProjectView key={`${ch}/${id}`} project={data} />;
}

function ProjectView({ project }: { project: Project }) {
  const key = `${project.chapter}/${project.id}`;
  const { progress, update, loaded } = useProgress();
  const { course } = useCourse();
  const { caps } = useCaps();
  const saved = progress.projects[key];
  const stepsDone = saved?.stepsDone ?? 0;
  const total = project.steps.length;
  const unlocked = Math.min(stepsDone + 1, total);
  const [current, setCurrent] = useState(Math.min(saved?.current ?? unlocked, unlocked));
  const [files, setFiles] = useState<SourceFile[]>(project.starter);
  const [grade, setGrade] = useState<GradeResponse>();
  const [busy, setBusy] = useState(false);
  const [err, setErr] = useState<string>();
  const [solution, setSolution] = useState<{ step: number; files: SourceFile[] }>();
  const [jump, setJump] = useState<{ file: string; line: number; nonce: number }>();
  const restored = useRef(false);
  const editable = project.meta.editable;
  useTrackLast(`Project: ${project.meta.title}`);

  useEffect(() => {
    if (!loaded || restored.current) return;
    restored.current = true;
    if (saved?.code?.length)
      setFiles(project.starter.map((f) => saved.code!.find((s) => s.name === f.name && editable.includes(f.name)) ?? f));
    setCurrent(Math.min(saved?.current ?? Math.min((saved?.stepsDone ?? 0) + 1, total), Math.min((saved?.stepsDone ?? 0) + 1, total)));
  }, [loaded, saved, project, editable, total]);

  const patch = (fn: (p: NonNullable<typeof saved>) => NonNullable<typeof saved>) =>
    update((p) => ({ ...p, projects: { ...p.projects, [key]: fn(p.projects[key] ?? { stepsDone: 0 }) } }));

  const timer = useRef<number | undefined>(undefined);
  const onChange = (fs: SourceFile[]) => {
    setFiles(fs);
    window.clearTimeout(timer.current);
    timer.current = window.setTimeout(() => patch((s) => ({ ...s, code: fs.filter((f) => editable.includes(f.name)) })), 800);
  };

  const selectStep = (n: number) => {
    if (n > unlocked) return;
    setCurrent(n);
    setGrade(undefined);
    setSolution(undefined);
    patch((s) => ({ ...s, current: n }));
  };

  const check = async () => {
    setBusy(true);
    setErr(undefined);
    try {
      const r = await api.grade({ kind: "project", id: key, step: current, files: files.filter((f) => editable.includes(f.name)) });
      setGrade(r);
      patch((s) => ({
        ...s,
        stepsDone: r.passed ? Math.max(s.stepsDone, current) : s.stepsDone,
        code: files.filter((f) => editable.includes(f.name)),
        attempts: { ...(s.attempts ?? {}), [current]: (s.attempts?.[current] ?? 0) + 1 },
      }));
    } catch (e) {
      setErr(e instanceof Error ? e.message : String(e));
    } finally {
      setBusy(false);
    }
  };

  const step = project.steps[current - 1];
  const hintsShown = saved?.hintsShown?.[current] ?? 0;
  const attempted = (saved?.attempts?.[current] ?? 0) > 0 || stepsDone >= current;
  const missing = missingCaps(caps, project.meta.requires);
  const chapter = course?.chapters.find((c) => c.id === project.chapter);
  const jumpTo = (d: Diagnostic) => setJump({ file: d.file, line: d.line, nonce: Date.now() });

  return (
    <div className="space-y-5" onKeyDown={(e) => {
      if ((e.metaKey || e.ctrlKey) && e.key === "Enter") { e.preventDefault(); if (!busy) check(); }
    }}>
      <header>
        <div className="text-sm text-zinc-500">
          <Link to={`/ch/${project.chapter}`} className="hover:underline">
            Chapter {chapter?.number} · Mini-project
          </Link>
        </div>
        <div className="flex flex-wrap items-center gap-3">
          <h1 className="text-2xl font-bold tracking-tight">{project.meta.title}</h1>
          {stepsDone >= total && <Badge tone="ok">completed</Badge>}
          <Badge tone="info">{project.meta.sanitizer === "tsan" ? "TSan" : project.meta.sanitizer === "asan_ubsan" ? "ASan + UBSan" : "no sanitizer"}</Badge>
        </div>
        <p className="mt-1 max-w-3xl text-zinc-600 dark:text-zinc-400">{project.meta.summary}</p>
      </header>
      {missing.length > 0 && (
        <div className="rounded-md border border-amber-400 bg-amber-50 p-2 text-sm text-amber-900 dark:bg-amber-950/40 dark:text-amber-200">
          {missing.map((m) => <div key={m}>⚠️ {CAP_HELP[m]}</div>)}
        </div>
      )}

      <ol className="flex flex-wrap gap-2">
        {project.steps.map((s) => {
          const done = stepsDone >= s.number;
          const locked = s.number > unlocked;
          return (
            <li key={s.number}>
              <button
                onClick={() => selectStep(s.number)}
                disabled={locked}
                title={locked ? "Pass the previous step to unlock" : ""}
                className={`flex items-center gap-2 rounded-lg border px-3 py-1.5 text-sm ${
                  s.number === current
                    ? "border-[#004482] bg-[#004482] text-white"
                    : done
                      ? "border-emerald-400 text-emerald-700 dark:text-emerald-400"
                      : locked
                        ? "border-zinc-200 text-zinc-400 dark:border-zinc-800"
                        : "border-zinc-300 dark:border-zinc-700"
                }`}
              >
                <span className="font-mono">{locked ? "🔒" : done ? "✓" : s.number}</span>
                {s.title}
              </button>
            </li>
          );
        })}
      </ol>

      <section className="rounded-xl border border-zinc-200 p-4 dark:border-zinc-800">
        <h2 className="mb-2 font-semibold">
          Step {current}/{total}: {step.title}
          {current > 1 && step.cumulative && <span className="ml-2 text-xs font-normal text-zinc-500">(tests of earlier steps run too)</span>}
        </h2>
        <Markdown chapter={project.chapter}>{step.statement}</Markdown>
      </section>

      <CodeEditor
        files={files}
        onChange={onChange}
        readOnlyFiles={project.starter.map((f) => f.name).filter((n) => !editable.includes(n))}
        diagnostics={grade?.compile.diagnostics}
        jumpTo={jump}
        minLines={18}
        maxLines={40}
      />

      <div className="flex flex-wrap items-center gap-2">
        <button
          onClick={check}
          disabled={busy}
          className="rounded-md bg-[#004482] px-4 py-1.5 text-sm font-medium text-white shadow-sm hover:bg-[#00376a] disabled:opacity-50"
          title="Ctrl/Cmd+Enter"
        >
          {busy ? "Checking…" : `✓ Check step ${current}`}
        </button>
        {step.hints.length > 0 && (
          <button
            onClick={() => patch((s) => ({ ...s, hintsShown: { ...(s.hintsShown ?? {}), [current]: hintsShown + 1 } }))}
            disabled={hintsShown >= step.hints.length}
            className="rounded-md border border-zinc-300 px-3 py-1.5 text-sm disabled:opacity-40 dark:border-zinc-700"
          >
            💡 Hint ({hintsShown}/{step.hints.length})
          </button>
        )}
        <button
          onClick={async () => {
            if (stepsDone < current && !confirm("Reveal the reference solution for this step?")) return;
            setSolution({ step: current, files: (await api.projectSolution(project.chapter, project.id, current)).files });
          }}
          disabled={!attempted}
          title={attempted ? "" : "Check this step at least once first"}
          className="rounded-md border border-zinc-300 px-3 py-1.5 text-sm disabled:opacity-40 dark:border-zinc-700"
        >
          🔑 Step solution
        </button>
        <button
          onClick={() => confirm("Reset all files to the project's starter code?") && onChange(project.starter)}
          className="rounded-md px-2 py-1.5 text-sm text-zinc-500 hover:text-zinc-800 dark:hover:text-zinc-200"
        >
          ↺ Reset code
        </button>
        {grade?.passed && current < total && (
          <button onClick={() => selectStep(current + 1)} className="ml-auto text-sm text-sky-600 hover:underline dark:text-sky-400">
            Next step: {project.steps[current].title} →
          </button>
        )}
      </div>

      {hintsShown > 0 && (
        <ol className="list-decimal space-y-1 rounded-md border border-amber-300 bg-amber-50 py-2 pl-8 pr-3 text-sm dark:border-amber-900 dark:bg-amber-950/30">
          {step.hints.slice(0, hintsShown).map((h, i) => (
            <li key={i} dangerouslySetInnerHTML={{ __html: inlineMd(h) }} />
          ))}
        </ol>
      )}
      {err && <div className="text-sm text-red-600">{err}</div>}
      {grade && grade.compile.ok && <TestResults result={grade} />}
      {grade && (!grade.compile.ok || grade.compile.diagnostics.length > 0 || grade.run?.stderr) && (
        <OutputPanel compile={grade.compile} run={grade.run?.stderr ? grade.run : undefined} onJump={jumpTo} hideStdout />
      )}
      {solution && solution.step === current && (
        <section className="space-y-2">
          <h2 className="font-semibold">Reference solution after step {current}</h2>
          <CodeEditor files={solution.files} readOnly />
        </section>
      )}
    </div>
  );
}
