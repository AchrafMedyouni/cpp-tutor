import { useEffect } from "react";
import { useLocation } from "react-router-dom";
import { useProgress } from "./progress";

/** Remember this page as "where I left off". */
export function useTrackLast(title: string | undefined) {
  const { update, loaded } = useProgress();
  const loc = useLocation();
  useEffect(() => {
    if (!title || !loaded) return;
    update((p) => ({ ...p, last: { route: loc.pathname, title, at: Date.now() } }));
  }, [title, loc.pathname, loaded, update]);
}
