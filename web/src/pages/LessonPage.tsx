import { useEffect } from "react";
import { Link, useParams } from "react-router-dom";
import { Markdown } from "../components/Markdown";
import { api } from "../lib/api";
import { useCourse } from "../lib/course";
import { useProgress } from "../lib/progress";
import { useAsync } from "../lib/useAsync";
import { useTrackLast } from "../lib/useLast";

export function LessonPage() {
  const { ch = "", id = "" } = useParams();
  const { course } = useCourse();
  const { progress, update } = useProgress();
  const { data, error } = useAsync(() => api.lesson(ch, id), [ch, id]);
  const chapter = course?.chapters.find((c) => c.id === ch);
  useTrackLast(data ? `${chapter?.number ?? ""}. ${data.title}` : undefined);
  const key = `${ch}/${id}`;
  const read = !!progress.lessonsRead[key];
  const markRead = (v = true) => update((p) => ({ ...p, lessonsRead: { ...p.lessonsRead, [key]: v } }));

  // Mark as read once the reader reaches the end of the page.
  useEffect(() => {
    if (!data || read) return;
    const sentinel = document.getElementById("lesson-end");
    if (!sentinel) return;
    const obs = new IntersectionObserver((entries) => {
      if (entries.some((e) => e.isIntersecting)) markRead();
    });
    const t = setTimeout(() => obs.observe(sentinel), 1500);
    return () => {
      clearTimeout(t);
      obs.disconnect();
    };
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [data, read, key]);

  if (error) return <div className="text-red-600">{error}</div>;
  if (!data || !chapter) return <div className="h-64 animate-pulse rounded-xl bg-zinc-100 dark:bg-zinc-900" />;

  const idx = chapter.lessons.findIndex((l) => l.id === id);
  const prev = chapter.lessons[idx - 1];
  const next = chapter.lessons[idx + 1];
  const nextChapter = course?.chapters[course.chapters.findIndex((c) => c.id === ch) + 1];

  return (
    <article>
      <div className="mb-1 text-sm text-zinc-500">
        <Link to={`/ch/${ch}`} className="hover:underline">
          Chapter {chapter.number} · {chapter.title}
        </Link>
        {data.slides && <> · slides {data.slides}</>}
      </div>
      <h1 className="mb-6 text-3xl font-bold tracking-tight">{data.title}</h1>
      <Markdown chapter={ch}>{data.markdown}</Markdown>
      <div id="lesson-end" className="mt-10 flex flex-wrap items-center justify-between gap-3 border-t border-zinc-200 pt-4 dark:border-zinc-800">
        {prev ? (
          <Link to={`/ch/${ch}/lesson/${prev.id}`} className="text-sky-600 hover:underline dark:text-sky-400">
            ← {prev.title}
          </Link>
        ) : (
          <span />
        )}
        <label className="flex items-center gap-2 text-sm text-zinc-500">
          <input type="checkbox" checked={read} onChange={(e) => markRead(e.target.checked)} /> read
        </label>
        {next ? (
          <Link to={`/ch/${ch}/lesson/${next.id}`} onClick={() => markRead()} className="text-sky-600 hover:underline dark:text-sky-400">
            {next.title} →
          </Link>
        ) : chapter.quizCount > 0 ? (
          <Link to={`/ch/${ch}/quiz`} onClick={() => markRead()} className="text-sky-600 hover:underline dark:text-sky-400">
            Chapter quiz →
          </Link>
        ) : nextChapter ? (
          <Link to={`/ch/${nextChapter.id}`} onClick={() => markRead()} className="text-sky-600 hover:underline dark:text-sky-400">
            Next chapter →
          </Link>
        ) : (
          <span />
        )}
      </div>
    </article>
  );
}
