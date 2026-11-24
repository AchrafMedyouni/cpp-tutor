import { Link, useParams } from "react-router-dom";
import { api } from "../lib/api";
import { useAsync } from "../lib/useAsync";
import { useCourse } from "../lib/course";
import { useProgress } from "../lib/progress";
import { chapterStats } from "../lib/stats";
import { Difficulty } from "./ExercisePage";

export function ChapterPage() {
  const { ch } = useParams();
  const { course } = useCourse();
  const { progress } = useProgress();
  const { data: viz } = useAsync(() => api.vizList(), []);
  const c = course?.chapters.find((x) => x.id === ch);
  const chapterViz = viz?.filter((v) => v.chapter === ch) ?? [];
  if (!course) return null;
  if (!c) return <div>Unknown chapter.</div>;
  const st = chapterStats(c, progress);
  return (
    <div className="space-y-8">
      <header>
        <div className="text-sm text-zinc-500">
          Chapter {c.number} · slides {c.slides}
        </div>
        <h1 className="text-3xl font-bold tracking-tight">{c.title}</h1>
        <p className="mt-2 max-w-3xl text-zinc-600 dark:text-zinc-400">{c.summary}</p>
        <div className="mt-3 h-2 max-w-md overflow-hidden rounded-full bg-zinc-100 dark:bg-zinc-800">
          <div className="h-full bg-emerald-500" style={{ width: `${st.pct}%` }} />
        </div>
      </header>
      <Section title="📖 Lessons">
        <ol className="space-y-1">
          {c.lessons.map((l, i) => (
            <li key={l.id}>
              <Link to={`/ch/${c.id}/lesson/${l.id}`} className="flex items-center gap-3 rounded-md px-2 py-1.5 hover:bg-zinc-100 dark:hover:bg-zinc-900">
                <span className="w-6 text-right font-mono text-xs text-zinc-400">{i + 1}</span>
                <span className="flex-1">{l.title}</span>
                {l.slides && <span className="text-xs text-zinc-400">slides {l.slides}</span>}
                {progress.lessonsRead[`${c.id}/${l.id}`] && <span className="text-emerald-600">✓</span>}
              </Link>
            </li>
          ))}
        </ol>
      </Section>
      {c.quizCount > 0 && (
        <Section title="📝 Quiz">
          <Link to={`/ch/${c.id}/quiz`} className="text-sky-600 hover:underline dark:text-sky-400">
            {c.quizCount} questions
          </Link>
          {progress.quiz[c.id] && <span className="ml-3 text-sm text-zinc-500">best score {progress.quiz[c.id].best}/{c.quizCount}</span>}
        </Section>
      )}
      {c.exercises.length > 0 && (
        <Section title="🏋️ Exercises">
          <ul className="space-y-1">
            {c.exercises.map((e) => (
              <li key={e.id}>
                <Link to={`/ch/${c.id}/exercise/${e.id}`} className="flex items-center gap-3 rounded-md px-2 py-1.5 hover:bg-zinc-100 dark:hover:bg-zinc-900">
                  <Difficulty n={e.difficulty} />
                  <span className="flex-1">{e.title}</span>
                  {e.requires.length > 0 && <span className="text-xs text-zinc-400">needs {e.requires.join(", ")}</span>}
                  {progress.exercises[`${c.id}/${e.id}`]?.passed && <span className="text-emerald-600">✓ passed</span>}
                </Link>
              </li>
            ))}
          </ul>
        </Section>
      )}
      {chapterViz.length > 0 && (
        <Section title="🧠 Memory visualizer">
          <ul className="space-y-1">
            {chapterViz.map((v) => (
              <li key={v.id}>
                <Link to={`/visualizer/${v.id}`} className="flex items-center gap-3 rounded-md px-2 py-1.5 hover:bg-zinc-100 dark:hover:bg-zinc-900">
                  <span className="flex-1">{v.title}</span>
                  <span className="text-xs text-zinc-500">{v.steps} steps</span>
                  {progress.vizSeen[v.id] && <span className="text-emerald-600">✓</span>}
                </Link>
              </li>
            ))}
          </ul>
        </Section>
      )}
      {c.projects.length > 0 && (
        <Section title="🛠 Mini-projects">
          <ul className="space-y-1">
            {c.projects.map((p) => {
              const done = progress.projects[`${c.id}/${p.id}`]?.stepsDone ?? 0;
              return (
                <li key={p.id}>
                  <Link to={`/ch/${c.id}/project/${p.id}`} className="flex items-center gap-3 rounded-md px-2 py-1.5 hover:bg-zinc-100 dark:hover:bg-zinc-900">
                    <span className="flex-1">{p.title}</span>
                    <span className="text-xs text-zinc-500">
                      {Math.min(done, p.steps)}/{p.steps} steps
                    </span>
                  </Link>
                </li>
              );
            })}
          </ul>
        </Section>
      )}
    </div>
  );
}

function Section({ title, children }: { title: string; children: React.ReactNode }) {
  return (
    <section>
      <h2 className="mb-2 text-lg font-semibold">{title}</h2>
      {children}
    </section>
  );
}
