import { useEffect, useState } from "react";

/** True while the page is in browser fullscreen (also tracks Esc / F11 exits). */
export function useIsFullscreen(): boolean {
  const [fs, setFs] = useState(() => !!document.fullscreenElement);
  useEffect(() => {
    const onChange = () => setFs(!!document.fullscreenElement);
    document.addEventListener("fullscreenchange", onChange);
    return () => document.removeEventListener("fullscreenchange", onChange);
  }, []);
  return fs;
}

export function toggleFullscreen() {
  if (document.fullscreenElement) document.exitFullscreen().catch(() => {});
  else document.documentElement.requestFullscreen().catch(() => {});
}

export const fullscreenSupported = () => !!document.fullscreenEnabled;
