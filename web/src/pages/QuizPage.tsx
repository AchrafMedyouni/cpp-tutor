import { useState } from "react";
import { Link, useParams } from "react-router-dom";
import { inlineMd } from "../components/ExampleRunner";
import { Markdown } from "../components/Markdown";
import { api, type QuizQuestion } from "../lib/api";
import { useCourse } from "../lib/course";
import { useProgress } from "../lib/progress";
import { useAsync } from "../lib/useAsync";
import { useTrackLast } from "../lib/useLast";

const KIND_LABEL: Record<QuizQuestion["kind"], string> = {
  output: "What does this print?",
  compiles: "Does this compile?",
  ub: "Is this UB?",
  "which-ctor": "Which constructor?",
  concept: "Concept",
};

export function QuizPage() {
  const { ch = "" } = useParams();
  const { course } = useCourse();
  const { data, error } = useAsync(() => api.quiz(ch), [ch]);
  const { progress, update } = useProgress();
  const [answers, setAnswers] = useState<Record<string, number>>({});
  const [submitted, setSubmitted] = useState(false);
  const chapter = course?.chapters.find((c) => c.id === ch);
  useTrackLast(chapter ? `Quiz: ${chapter.title}` : undefined);
  if (error) return <div className="text-red-600">{error}</div>;
  if (!data || !chapter) return <div className="h-64 animate-pulse rounded-xl bg-zinc-100 dark:bg-zinc-900" />;
  const qs = data.questions;
  const score = qs.filter((q) => answers[q.id] === q.answer).length;
  const best = progress.quiz[ch]?.best;

  const submit = () => {
    setSubmitted(true);
    update((p) => ({
      ...p,
      quiz: { ...p.quiz, [ch]: { best: Math.max(p.quiz[ch]?.best ?? 0, score), total: qs.length, answers } },
    }));
    window.scrollTo({ top: 0, behavior: "smooth" });
  };

  return (
    <div className="space-y-6">
      <header>
        <div className="text-sm text-zinc-500">
          <Link to={`/ch/${ch}`} className="hover:underline">Chapter {chapter.number} · {chapter.title}</Link>
        </div>
        <h1 className="text-2xl font-bold tracking-tight">Quiz</h1>
        <p className="text-sm text-zinc-500">
          {qs.length} questions{best !== undefined && ` · best score ${best}/${qs.length}`}
        </p>
      </header>
      {submitted && (
        <div className={`rounded-lg border p-4 text-lg font-semibold ${score === qs.length ? "border-emerald-400 bg-emerald-50 text-emerald-800 dark:bg-emerald-950/40 dark:text-emerald-300" : "border-zinc-300 dark:border-zinc-700"}`}>
          Score: {score}/{qs.length} {score === qs.length && "🎉"}
          <button
            onClick={() => {
              setAnswers({});
              setSubmitted(false);
            }}
            className="ml-4 text-sm font-normal text-sky-600 hover:underline dark:text-sky-400"
          >
            Retry
          </button>
        </div>
      )}
      <ol className="space-y-6">
        {qs.map((q, i) => {
          const chosen = answers[q.id];
          return (
            <li key={q.id} className="rounded-xl border border-zinc-200 p-4 dark:border-zinc-800">
              <div className="mb-2 flex items-center gap-2 text-xs text-zinc-500">
                <span className="font-mono">Q{i + 1}</span>
                <span className="rounded bg-zinc-100 px-1.5 py-0.5 dark:bg-zinc-800">{KIND_LABEL[q.kind]}</span>
                {q.slides && <span>slides {q.slides}</span>}
              </div>
              <div className="mb-3 font-medium" dangerouslySetInnerHTML={{ __html: inlineMd(q.question) }} />
              {q.code && <Markdown chapter={ch}>{"```cpp\n" + q.code + "\n```"}</Markdown>}
              <div className="mt-3 grid gap-2">
                {q.choices.map((c, j) => {
                  const isChosen = chosen === j;
                  const correct = submitted && j === q.answer;
                  const wrong = submitted && isChosen && j !== q.answer;
                  return (
                    <button
                      key={j}
                      disabled={submitted}
                      onClick={() => setAnswers({ ...answers, [q.id]: j })}
                      className={`rounded-lg border px-3 py-2 text-left text-sm transition ${
                        correct
                          ? "border-emerald-500 bg-emerald-50 dark:bg-emerald-950/40"
                          : wrong
                            ? "border-red-500 bg-red-50 dark:bg-red-950/40"
                            : isChosen
                              ? "border-sky-500 bg-sky-50 dark:bg-sky-950/40"
                              : "border-zinc-200 hover:border-zinc-400 dark:border-zinc-800 dark:hover:border-zinc-600"
                      }`}
                    >
                      <span dangerouslySetInnerHTML={{ __html: inlineMd(c) }} />
                    </button>
                  );
                })}
              </div>
              {submitted && (
                <div className="mt-3 rounded-md bg-zinc-50 p-3 text-sm dark:bg-zinc-900">
                  <b>{chosen === q.answer ? "✓ Correct." : chosen === undefined ? "Not answered." : "✗ Not quite."}</b>{" "}
                  <span dangerouslySetInnerHTML={{ __html: inlineMd(q.explanation) }} />
                </div>
              )}
            </li>
          );
        })}
      </ol>
      {!submitted && (
        <button
          onClick={submit}
          className="rounded-lg bg-[#004482] px-5 py-2 font-medium text-white hover:bg-[#00376a]"
        >
          Submit answers ({Object.keys(answers).length}/{qs.length} answered)
        </button>
      )}
    </div>
  );
}
