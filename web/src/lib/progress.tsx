import { createContext, useCallback, useContext, useEffect, useRef, useState, type ReactNode } from "react";
import { api, type SourceFile } from "./api";

export interface Progress {
  lessonsRead: Record<string, boolean>; // "ch/lesson"
  quiz: Record<string, { best: number; total: number; answers: Record<string, number> }>; // by chapter
  exercises: Record<
    string,
    { passed: boolean; attempts: number; code?: SourceFile[]; hintsShown?: number; solutionShown?: boolean }
  >; // "ch/id"
  projects: Record<
    string,
    { stepsDone: number; current?: number; code?: SourceFile[]; hintsShown?: Record<number, number>; attempts?: Record<number, number> }
  >;
  vizSeen: Record<string, boolean>;
  last?: { route: string; title: string; at: number };
}

export const emptyProgress = (): Progress => ({ lessonsRead: {}, quiz: {}, exercises: {}, projects: {}, vizSeen: {} });

interface Ctx {
  progress: Progress;
  loaded: boolean;
  update: (fn: (p: Progress) => Progress) => void;
  reset: () => void;
}
const ProgressContext = createContext<Ctx | null>(null);

export function ProgressProvider({ children }: { children: ReactNode }) {
  const [progress, setProgress] = useState<Progress>(emptyProgress);
  const [loaded, setLoaded] = useState(false);
  const timer = useRef<number | undefined>(undefined);

  useEffect(() => {
    api
      .getProgress()
      .then((p) => setProgress({ ...emptyProgress(), ...(p as Partial<Progress>) }))
      .catch(() => {
        const local = localStorage.getItem("cpplab-progress");
        if (local) setProgress({ ...emptyProgress(), ...JSON.parse(local) });
      })
      .finally(() => setLoaded(true));
  }, []);

  const save = useCallback((p: Progress) => {
    window.clearTimeout(timer.current);
    timer.current = window.setTimeout(() => {
      localStorage.setItem("cpplab-progress", JSON.stringify(p)); // backup copy
      api.putProgress(p).catch(() => {});
    }, 400);
  }, []);

  const update = useCallback(
    (fn: (p: Progress) => Progress) => {
      setProgress((prev) => {
        const next = fn(prev);
        save(next);
        return next;
      });
    },
    [save],
  );

  const reset = useCallback(() => {
    const p = emptyProgress();
    setProgress(p);
    localStorage.removeItem("cpplab-progress");
    api.putProgress(p).catch(() => {});
  }, []);

  return <ProgressContext.Provider value={{ progress, loaded, update, reset }}>{children}</ProgressContext.Provider>;
}

export function useProgress(): Ctx {
  const c = useContext(ProgressContext);
  if (!c) throw new Error("useProgress outside provider");
  return c;
}
