import { useEffect, useState } from "react";

export function useIsDark(): boolean {
  const [dark, setDark] = useState(() => document.documentElement.classList.contains("dark"));
  useEffect(() => {
    const obs = new MutationObserver(() => setDark(document.documentElement.classList.contains("dark")));
    obs.observe(document.documentElement, { attributes: true, attributeFilter: ["class"] });
    return () => obs.disconnect();
  }, []);
  return dark;
}

export function toggleTheme() {
  const dark = document.documentElement.classList.toggle("dark");
  localStorage.setItem("cpplab-theme", dark ? "dark" : "light");
}
