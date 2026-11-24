import type { ChapterOverview } from "./api";
import type { Progress } from "./progress";

export interface ChapterStats {
  lessons: [number, number];
  exercises: [number, number];
  projectSteps: [number, number];
  quiz: [number, number]; // best score / count
  pct: number;
}

export function chapterStats(ch: ChapterOverview, p: Progress): ChapterStats {
  const lessons = ch.lessons.filter((l) => p.lessonsRead[`${ch.id}/${l.id}`]).length;
  const exercises = ch.exercises.filter((e) => p.exercises[`${ch.id}/${e.id}`]?.passed).length;
  const stepsTotal = ch.projects.reduce((n, pr) => n + pr.steps, 0);
  const stepsDone = ch.projects.reduce((n, pr) => n + Math.min(p.projects[`${ch.id}/${pr.id}`]?.stepsDone ?? 0, pr.steps), 0);
  const quizBest = p.quiz[ch.id]?.best ?? 0;
  const parts: [number, number][] = [
    [lessons, ch.lessons.length],
    [exercises, ch.exercises.length],
    [stepsDone, stepsTotal],
    [quizBest, ch.quizCount],
  ];
  const done = parts.reduce((n, [d]) => n + d, 0);
  const total = parts.reduce((n, [, t]) => n + t, 0);
  return {
    lessons: [lessons, ch.lessons.length],
    exercises: [exercises, ch.exercises.length],
    projectSteps: [stepsDone, stepsTotal],
    quiz: [quizBest, ch.quizCount],
    pct: total ? Math.round((100 * done) / total) : 0,
  };
}
