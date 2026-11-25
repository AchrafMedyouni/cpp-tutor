import { useCallback, useEffect, useState } from "react";
import { Link, useParams } from "react-router-dom";
import { drillProgram } from "../../../shared/drill";
import { CodeEditor } from "../components/CodeEditor";
import { inlineMd } from "../components/ExampleRunner";
import { Markdown } from "../components/Markdown";
import { OutputPanel } from "../components/OutputPanel";
import { api, type Drill, type RunResponse } from "../lib/api";
import { useCourse } from "../lib/course";
import { useProgress } from "../lib/progress";
import { useAsync } from "../lib/useAsync";
import { useTrackLast } from "../lib/useLast";

type Key = `${number}:${number}`;
const keyOf = (s: number, c: number): Key => `${s}:${c}`;
const parse = (k: Key) => k.split(":").map(Number) as [number, number];

export function DrillPage() {
  const { ch = "" } = useParams();
  const { data, error } = useAsync(() => api.drill(ch), [ch]);
  if (error) return <div className="text-red-600">{error}</div>;
  if (!data) return <div className="h-64 animate-pulse rounded-xl bg-zinc-100 dark:bg-zinc-900" />;
  return <DrillView key={ch} ch={ch} drill={data} />;
}

function DrillView({ ch, drill }: { ch: string; drill: Drill }) {
  const { course } = useCourse();
  const { progress, update } = useProgress();
  const chapter = course?.chapters.find((c) => c.id === ch);
  useTrackLast(drill.title);
  const best = progress.drills?.[ch]?.best ?? {};

  const [queue, setQueue] = useState<Key[] | null>(null); // null = section menu
  const [pos, setPos] = useState(0);
  const [results, setResults] = useState<Record<Key, boolean>>({}); // this session: card -> answered right?
  const [answer, setAnswer] = useState<boolean | null>(null); // what the learner said for the current card
  const [compiler, setCompiler] = useState<RunResponse | "loading" | null>(null);

  const start = (keys: Key[]) => {
    setQueue(keys);
    setPos(0);
    setAnswer(null);
    setCompiler(null);
    window.scrollTo(0, 0);
  };
  const sectionKeys = (s: number) => drill.sections[s].cards.map((_, c) => keyOf(s, c));

  const finished = queue !== null && pos >= queue.length;
  const current = queue && !finished ? parse(queue[pos]) : null;
  const sec = current ? drill.sections[current[0]] : null;
  const card = current && sec ? sec.cards[current[1]] : null;

  const choose = useCallback(
    (saysCompiles: boolean) => {
      if (!card || !queue || answer !== null) return;
      setAnswer(saysCompiles);
      setResults((r) => ({ ...r, [queue[pos]]: saysCompiles === card.compiles }));
    },
    [card, queue, pos, answer],
  );
  const next = useCallback(() => {
    if (answer === null) return;
    setAnswer(null);
    setCompiler(null);
    setPos((p) => p + 1);
  }, [answer]);

  // Record the best score of every section fully answered in this session.
  useEffect(() => {
    if (!finished) return;
    update((p) => {
      const prev = p.drills?.[ch]?.best ?? {};
      const nextBest = { ...prev };
      drill.sections.forEach((s, si) => {
        const keys = s.cards.map((_, c) => keyOf(si, c));
        if (keys.every((k) => k in results)) {
          const score = keys.filter((k) => results[k]).length;
          nextBest[si] = Math.max(prev[si] ?? 0, score);
        }
      });
      return { ...p, drills: { ...(p.drills ?? {}), [ch]: { best: nextBest } } };
    });
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [finished]);

  useEffect(() => {
    const onKey = (e: KeyboardEvent) => {
      if (e.metaKey || e.ctrlKey || e.altKey) return;
      const t = e.target as HTMLElement | null;
      if (t?.closest(".monaco-editor") || t?.tagName === "INPUT" || t?.tagName === "TEXTAREA") return;
      const k = e.key.toLowerCase();
      if (k === "c") choose(true);
      else if (k === "e") choose(false);
      else if (k === "enter" && answer !== null) {
        e.preventDefault();
        next();
      }
    };
    window.addEventListener("keydown", onKey);
    return () => window.removeEventListener("keydown", onKey);
  }, [choose, next, answer]);

  const askCompiler = async () => {
    if (!card || !sec) return;
    setCompiler("loading");
    try {
      setCompiler(await api.run({ files: [{ name: "main.cpp", content: drillProgram(sec.prelude, card) }], sanitizer: "none" }));
    } catch (e) {
      setCompiler(null);
      alert(e instanceof Error ? e.message : String(e));
    }
  };

  const header = (
    <header>
      <div className="text-sm text-zinc-500">
        <Link to={`/ch/${ch}`} className="hover:underline">
          Chapter {chapter?.number} · {chapter?.title}
        </Link>
      </div>
      <h1 className="text-2xl font-bold tracking-tight">⚡ {drill.title}</h1>
    </header>
  );

  // ------------------------------------------------------------ section menu
  if (queue === null) {
    const total = drill.sections.reduce((n, s) => n + s.cards.length, 0);
    return (
      <div className="space-y-6">
        {header}
        <Markdown chapter={ch}>{drill.intro}</Markdown>
        <ul className="space-y-2">
          {drill.sections.map((s, si) => {
            const b = best[si];
            const perfect = b === s.cards.length;
            return (
              <li key={si} className="flex flex-wrap items-center gap-3 rounded-xl border border-zinc-200 p-3 dark:border-zinc-800">
                <span className="w-6 text-right font-mono text-sm text-zinc-400">{si + 1}</span>
                <span className="flex-1 font-medium" dangerouslySetInnerHTML={{ __html: inlineMd(s.title) }} />
                <span className={`text-sm ${perfect ? "text-emerald-600" : "text-zinc-500"}`}>
                  {b === undefined ? `${s.cards.length} cards` : `best ${b}/${s.cards.length}${perfect ? " ✓" : ""}`}
                </span>
                <button onClick={() => start(sectionKeys(si))} className="rounded-md bg-[#004482] px-3 py-1 text-sm font-medium text-white hover:bg-[#00376a]">
                  Start
                </button>
              </li>
            );
          })}
        </ul>
        <button
          onClick={() => start(drill.sections.flatMap((_, si) => sectionKeys(si)))}
          className="rounded-md border border-zinc-300 px-3 py-1.5 text-sm dark:border-zinc-700"
        >
          Do all {total} cards in a row
        </button>
      </div>
    );
  }

  // ------------------------------------------------------------ results
  if (finished) {
    const missed = queue.filter((k) => !results[k]);
    const right = queue.length - missed.length;
    return (
      <div className="space-y-6">
        {header}
        <div
          className={`rounded-lg border p-4 text-lg font-semibold ${missed.length === 0 ? "border-emerald-400 bg-emerald-50 text-emerald-800 dark:bg-emerald-950/40 dark:text-emerald-300" : "border-zinc-300 dark:border-zinc-700"}`}
        >
          {right}/{queue.length} right {missed.length === 0 && "🎉"}
        </div>
        {missed.length > 0 && (
          <section className="space-y-3">
            <h2 className="font-semibold">The ones you missed: re-read the explanation, then retry them</h2>
            {missed.map((k) => {
              const [si, ci] = parse(k);
              const s = drill.sections[si];
              const c = s.cards[ci];
              return (
                <div key={k} className="rounded-lg border border-zinc-200 p-3 dark:border-zinc-800">
                  <div className="mb-1 text-xs text-zinc-500" dangerouslySetInnerHTML={{ __html: inlineMd(s.title) }} />
                  <Markdown chapter={ch}>{"```cpp\n" + c.code + "\n```"}</Markdown>
                  <p className="mt-1 text-sm">
                    <b>{c.compiles ? "✓ Compiles." : "✗ Compile error."}</b>{" "}
                    <span dangerouslySetInnerHTML={{ __html: inlineMd(c.explanation) }} />
                  </p>
                </div>
              );
            })}
          </section>
        )}
        <div className="flex flex-wrap gap-2">
          {missed.length > 0 && (
            <button onClick={() => start(missed)} className="rounded-md bg-[#004482] px-4 py-1.5 text-sm font-medium text-white hover:bg-[#00376a]">
              Retry the {missed.length} missed
            </button>
          )}
          <button onClick={() => setQueue(null)} className="rounded-md border border-zinc-300 px-3 py-1.5 text-sm dark:border-zinc-700">
            Back to sections
          </button>
        </div>
      </div>
    );
  }

  // ------------------------------------------------------------ one card
  const right = answer !== null && answer === card!.compiles;
  const sessionRight = queue.slice(0, pos).filter((k) => results[k]).length;
  return (
    <div className="space-y-4">
      {header}
      <div className="flex items-center gap-3 text-sm text-zinc-500">
        <span dangerouslySetInnerHTML={{ __html: inlineMd(sec!.title) }} />
        <span className="ml-auto font-mono">
          {pos + 1}/{queue.length} · {sessionRight} right
        </span>
        <button onClick={() => setQueue(null)} className="hover:text-zinc-800 dark:hover:text-zinc-200">
          ✕ quit
        </button>
      </div>
      <div className="h-1 overflow-hidden rounded-full bg-zinc-100 dark:bg-zinc-800">
        <div className="h-full bg-[#004482] transition-all" style={{ width: `${(100 * pos) / queue.length}%` }} />
      </div>
      <details className="rounded-md border border-amber-300 bg-amber-50 px-3 py-2 text-sm dark:border-amber-900 dark:bg-amber-950/30">
        <summary className="cursor-pointer font-medium">Rule for this section</summary>
        <p className="mt-1" dangerouslySetInnerHTML={{ __html: inlineMd(sec!.rule) }} />
      </details>
      <div>
        <div className="mb-1 text-xs font-medium uppercase tracking-wide text-zinc-500">Given</div>
        <div className="text-sm opacity-80">
          <Markdown chapter={ch}>{"```cpp\n" + sec!.prelude + "\n```"}</Markdown>
        </div>
      </div>
      <div>
        <div className="mb-1 text-xs font-medium uppercase tracking-wide text-zinc-500">
          {card!.scope === "global" ? "This definition" : "These lines, inside main()"}: does it compile?
        </div>
        <div className="text-lg [&_pre]:!text-base">
          <Markdown chapter={ch}>{"```cpp\n" + card!.code + "\n```"}</Markdown>
        </div>
      </div>

      {answer === null ? (
        <div className="flex gap-3">
          <button onClick={() => choose(true)} className="flex-1 rounded-lg border-2 border-emerald-500 py-3 font-semibold text-emerald-700 hover:bg-emerald-50 dark:text-emerald-400 dark:hover:bg-emerald-950/40">
            ✓ Compiles <kbd className="ml-1 text-xs opacity-60">C</kbd>
          </button>
          <button onClick={() => choose(false)} className="flex-1 rounded-lg border-2 border-red-500 py-3 font-semibold text-red-700 hover:bg-red-50 dark:text-red-400 dark:hover:bg-red-950/40">
            ✗ Compile error <kbd className="ml-1 text-xs opacity-60">E</kbd>
          </button>
        </div>
      ) : (
        <div className={`rounded-lg border p-4 ${right ? "border-emerald-400 bg-emerald-50 dark:bg-emerald-950/30" : "border-red-400 bg-red-50 dark:bg-red-950/30"}`}>
          <div className="mb-1 font-semibold">
            {right ? "✓ Right" : "✗ Not quite"}: it {card!.compiles ? "compiles" : "does not compile"}.
          </div>
          <p className="text-sm" dangerouslySetInnerHTML={{ __html: inlineMd(card!.explanation) }} />
          <div className="mt-3 flex flex-wrap gap-2">
            <button onClick={next} className="rounded-md bg-[#004482] px-4 py-1.5 text-sm font-medium text-white hover:bg-[#00376a]">
              {pos + 1 < queue.length ? "Next" : "See results"} → <kbd className="ml-1 text-xs opacity-70">Enter</kbd>
            </button>
            {compiler === null && (
              <button onClick={askCompiler} className="rounded-md border border-zinc-300 px-3 py-1.5 text-sm dark:border-zinc-700">
                🔍 What does the compiler say?
              </button>
            )}
          </div>
        </div>
      )}
      {compiler === "loading" && <div className="text-sm text-zinc-500">Compiling…</div>}
      {compiler && compiler !== "loading" && (
        <section className="space-y-2">
          <p className="text-xs text-zinc-500">The full program the compiler sees (the <i>Given</i> code + the card):</p>
          <CodeEditor files={[{ name: "main.cpp", content: drillProgram(sec!.prelude, card!) }]} readOnly diagnostics={compiler.compile.diagnostics} maxLines={30} />
          <OutputPanel compile={compiler.compile} run={compiler.run} />
        </section>
      )}
    </div>
  );
}
