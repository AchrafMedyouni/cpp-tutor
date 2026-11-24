import { useEffect, useState } from "react";
import { Link, NavLink, Outlet, useLocation, useMatch } from "react-router-dom";
import { useCaps } from "../lib/caps";
import { useCourse } from "../lib/course";
import { useProgress } from "../lib/progress";
import { chapterStats } from "../lib/stats";
import { toggleTheme, useIsDark } from "../lib/theme";

export function Layout() {
  const { course, error } = useCourse();
  const { caps, error: capsErr } = useCaps();
  const { progress } = useProgress();
  const ch = useMatch("/ch/:ch/*")?.params.ch;
  const dark = useIsDark();
  const loc = useLocation();
  const [open, setOpen] = useState<string | undefined>(ch);
  const [mobileNav, setMobileNav] = useState(false);
  useEffect(() => {
    setOpen(ch);
  }, [ch]);
  useEffect(() => {
    window.scrollTo(0, 0);
    setMobileNav(false);
  }, [loc.pathname]);

  return (
    <div className="flex min-h-screen">
      {mobileNav && <div className="fixed inset-0 z-30 bg-black/40 lg:hidden" onClick={() => setMobileNav(false)} />}
      <aside
        className={`${mobileNav ? "fixed inset-y-0 left-0 z-40 block bg-white dark:bg-zinc-950" : "hidden"} h-screen w-72 shrink-0 overflow-y-auto border-r border-zinc-200 bg-zinc-50/70 px-3 py-4 text-sm lg:sticky lg:top-0 lg:block dark:border-zinc-800 dark:bg-zinc-900/40`}
      >
        <Link to="/" className="mb-4 flex items-center gap-2 px-2">
          <span className="rounded bg-[#004482] px-1.5 py-0.5 font-mono text-xs font-bold text-white">C++</span>
          <span className="font-semibold">CppLab</span>
          <span className="text-xs text-zinc-500">3MD1080</span>
        </Link>
        <nav className="space-y-0.5">
          <SideLink to="/" end>
            🏠 Dashboard
          </SideLink>
          <SideLink to="/visualizer">🧠 Memory visualizer</SideLink>
          <div className="my-3 border-t border-zinc-200 dark:border-zinc-800" />
          {course?.chapters.map((c) => {
            const st = chapterStats(c, progress);
            const isOpen = open === c.id;
            return (
              <div key={c.id}>
                <button
                  onClick={() => setOpen(isOpen ? undefined : c.id)}
                  className={`flex w-full items-center gap-2 rounded-md px-2 py-1.5 text-left hover:bg-zinc-200/60 dark:hover:bg-zinc-800 ${ch === c.id ? "font-semibold" : ""}`}
                >
                  <span className="w-5 text-right font-mono text-xs text-zinc-400">{c.number}</span>
                  <span className="flex-1">{c.title}</span>
                  <span className={`text-xs ${st.pct === 100 ? "text-emerald-600" : "text-zinc-400"}`}>{st.pct}%</span>
                </button>
                {isOpen && (
                  <div className="mb-2 ml-7 space-y-0.5 border-l border-zinc-200 pl-2 dark:border-zinc-800">
                    <SideLink to={`/ch/${c.id}`} end>
                      Overview
                    </SideLink>
                    {c.lessons.map((l) => (
                      <SideLink key={l.id} to={`/ch/${c.id}/lesson/${l.id}`} done={progress.lessonsRead[`${c.id}/${l.id}`]}>
                        {l.title}
                      </SideLink>
                    ))}
                    {c.quizCount > 0 && (
                      <SideLink to={`/ch/${c.id}/quiz`} done={(progress.quiz[c.id]?.best ?? 0) === c.quizCount}>
                        📝 Quiz
                      </SideLink>
                    )}
                    {c.exercises.map((e) => (
                      <SideLink key={e.id} to={`/ch/${c.id}/exercise/${e.id}`} done={progress.exercises[`${c.id}/${e.id}`]?.passed}>
                        🏋️ {e.title}
                      </SideLink>
                    ))}
                    {c.projects.map((p) => (
                      <SideLink
                        key={p.id}
                        to={`/ch/${c.id}/project/${p.id}`}
                        done={(progress.projects[`${c.id}/${p.id}`]?.stepsDone ?? 0) >= p.steps}
                      >
                        🛠 {p.title}
                      </SideLink>
                    ))}
                  </div>
                )}
              </div>
            );
          })}
        </nav>
        <div className="mt-6 border-t border-zinc-200 px-2 pt-3 text-xs text-zinc-500 dark:border-zinc-800">
          <button onClick={toggleTheme} className="hover:text-zinc-800 dark:hover:text-zinc-200">
            {dark ? "☀️ Light mode" : "🌙 Dark mode"}
          </button>
          {caps?.compiler && (
            <div className="mt-2" title={caps.compiler.path}>
              {caps.compiler.version.replace(/\(.*\)/, "").trim()}
              <br />
              par: {caps.par.available ? "✓" : "✗"} · tsan: {caps.tsan ? "✓" : "✗"} · tbb: {caps.tbb.available ? "✓" : "✗"}
            </div>
          )}
        </div>
      </aside>
      <main className="min-w-0 flex-1">
        <div className="sticky top-0 z-20 flex items-center gap-3 border-b border-zinc-200 bg-white/90 px-4 py-2 backdrop-blur lg:hidden dark:border-zinc-800 dark:bg-zinc-950/90">
          <button onClick={() => setMobileNav(true)} className="rounded-md border border-zinc-300 px-2 py-1 text-sm dark:border-zinc-700" aria-label="Open navigation">
            ☰
          </button>
          <Link to="/" className="font-semibold">CppLab</Link>
        </div>
        {(capsErr || (caps && !caps.compiler) || caps?.error) && (
          <div className="border-b border-red-300 bg-red-50 px-6 py-3 text-sm text-red-800 dark:border-red-900 dark:bg-red-950/50 dark:text-red-300">
            <b>⚠️ Compiler problem:</b>{" "}
            {capsErr ? `Cannot reach the CppLab server (${capsErr}). Is \`npm run dev\` running?` : caps?.error}
          </div>
        )}
        {error && <div className="p-6 text-red-600">Failed to load course: {error}</div>}
        <div className="mx-auto max-w-5xl px-6 py-8">
          <Outlet />
        </div>
      </main>
    </div>
  );
}

function SideLink({ to, children, end, done }: { to: string; children: React.ReactNode; end?: boolean; done?: boolean }) {
  return (
    <NavLink
      to={to}
      end={end}
      className={({ isActive }) =>
        `flex items-center gap-1.5 rounded-md px-2 py-1 ${isActive ? "bg-zinc-200 font-medium text-zinc-900 dark:bg-zinc-800 dark:text-zinc-100" : "text-zinc-600 hover:bg-zinc-200/60 dark:text-zinc-400 dark:hover:bg-zinc-800"}`
      }
    >
      <span className="flex-1 truncate">{children}</span>
      {done && <span className="text-emerald-600">✓</span>}
    </NavLink>
  );
}
