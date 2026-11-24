import { Link } from "react-router-dom";
import { useCourse } from "../lib/course";
import { useProgress } from "../lib/progress";
import { chapterStats } from "../lib/stats";

export function Dashboard() {
  const { course } = useCourse();
  const { progress, reset } = useProgress();
  if (!course) return <div className="h-64 animate-pulse rounded-xl bg-zinc-100 dark:bg-zinc-900" />;

  const stats = course.chapters.map((c) => ({ c, s: chapterStats(c, progress) }));
  const sum = (k: "lessons" | "exercises" | "projectSteps" | "quiz") =>
    stats.reduce<[number, number]>(([d, t], { s }) => [d + s[k][0], t + s[k][1]], [0, 0]);
  const overall = Math.round(stats.reduce((n, { s }) => n + s.pct, 0) / Math.max(stats.length, 1));
  const first = course.chapters[0];
  const firstLesson = first?.lessons[0];
  const cont = progress.last ?? (firstLesson ? { route: `/ch/${first.id}/lesson/${firstLesson.id}`, title: firstLesson.title } : undefined);

  return (
    <div className="space-y-8">
      <header className="flex flex-wrap items-end justify-between gap-4">
        <div>
          <h1 className="text-3xl font-bold tracking-tight">{course.title}</h1>
          {course.subtitle && <p className="mt-1 text-zinc-500">{course.subtitle}</p>}
        </div>
        {cont && (
          <Link
            to={cont.route}
            className="rounded-lg bg-[#004482] px-4 py-2.5 font-medium text-white shadow hover:bg-[#00376a]"
          >
            {progress.last ? "Continue where you left off" : "Start the course"} →
            <div className="text-xs font-normal opacity-80">{cont.title}</div>
          </Link>
        )}
      </header>

      <section className="grid grid-cols-2 gap-3 md:grid-cols-5">
        <Stat label="Overall" value={`${overall}%`} />
        <Stat label="Lessons read" pair={sum("lessons")} />
        <Stat label="Quiz points (best)" pair={sum("quiz")} />
        <Stat label="Exercises passed" pair={sum("exercises")} />
        <Stat label="Project steps" pair={sum("projectSteps")} />
      </section>

      <section className="space-y-3">
        <h2 className="text-lg font-semibold">Chapters</h2>
        <div className="grid gap-3 md:grid-cols-2">
          {stats.map(({ c, s }) => (
            <Link
              key={c.id}
              to={`/ch/${c.id}`}
              className="group rounded-xl border border-zinc-200 p-4 hover:border-zinc-400 dark:border-zinc-800 dark:hover:border-zinc-600"
            >
              <div className="flex items-baseline justify-between gap-2">
                <div className="font-semibold">
                  <span className="mr-2 font-mono text-zinc-400">{c.number}</span>
                  {c.title}
                </div>
                <span className="text-xs text-zinc-500">slides {c.slides}</span>
              </div>
              <div className="mt-3 h-2 overflow-hidden rounded-full bg-zinc-100 dark:bg-zinc-800">
                <div className="h-full rounded-full bg-emerald-500 transition-all" style={{ width: `${s.pct}%` }} />
              </div>
              <div className="mt-2 flex flex-wrap gap-x-4 text-xs text-zinc-500">
                <span>📖 {s.lessons.join("/")}</span>
                <span>📝 {s.quiz.join("/")}</span>
                <span>🏋️ {s.exercises.join("/")}</span>
                {s.projectSteps[1] > 0 && <span>🛠 {s.projectSteps.join("/")}</span>}
                <span className="ml-auto font-medium">{s.pct}%</span>
              </div>
            </Link>
          ))}
        </div>
      </section>

      <section className="border-t border-zinc-200 pt-4 text-sm dark:border-zinc-800">
        <button
          onClick={() => {
            if (confirm("Reset all progress (lessons, quiz scores, exercises, saved code)? This cannot be undone.")) reset();
          }}
          className="text-red-600 hover:underline"
        >
          Reset all progress
        </button>
        <span className="ml-3 text-zinc-500">Progress is saved in data/progress.json.</span>
      </section>
    </div>
  );
}

function Stat({ label, value, pair }: { label: string; value?: string; pair?: [number, number] }) {
  return (
    <div className="rounded-xl border border-zinc-200 p-3 dark:border-zinc-800">
      <div className="text-xs text-zinc-500">{label}</div>
      <div className="text-2xl font-semibold tabular-nums">
        {value ?? (
          <>
            {pair![0]}
            <span className="text-base text-zinc-400">/{pair![1]}</span>
          </>
        )}
      </div>
    </div>
  );
}
