import type { SanitizerMode } from "../lib/api";
import { useCaps } from "../lib/caps";

export interface RunSettings {
  sanitizer: SanitizerMode;
  trackAllocs: boolean;
  stdin: string;
  showStdin: boolean;
}

export function RunControls({
  settings,
  onChange,
  onRun,
  running,
  label = "Run",
  extra,
}: {
  settings: RunSettings;
  onChange: (s: RunSettings) => void;
  onRun: () => void;
  running: boolean;
  label?: string;
  extra?: React.ReactNode;
}) {
  const { caps } = useCaps();
  const noCompiler = caps && !caps.compiler;
  return (
    <div className="space-y-2">
      <div className="flex flex-wrap items-center gap-2 text-sm">
        <button
          onClick={onRun}
          disabled={running || !!noCompiler}
          className="inline-flex items-center gap-1.5 rounded-md bg-emerald-600 px-3 py-1.5 font-medium text-white shadow-sm hover:bg-emerald-700 disabled:opacity-50"
          title="Compile and run (Ctrl/Cmd+Enter)"
        >
          {running ? <span className="h-3 w-3 animate-spin rounded-full border-2 border-white/40 border-t-white" /> : "▶"}
          {running ? "Running…" : label}
        </button>
        <select
          value={settings.sanitizer}
          onChange={(e) => onChange({ ...settings, sanitizer: e.target.value as SanitizerMode })}
          className="rounded-md border border-zinc-300 bg-white px-2 py-1.5 dark:border-zinc-700 dark:bg-zinc-900"
          title="Sanitizers detect memory errors, undefined behaviour and data races at run time"
        >
          <option value="none">No sanitizer</option>
          <option value="asan_ubsan" disabled={caps && !caps.asanUbsan}>
            ASan + UBSan
          </option>
          <option value="tsan" disabled={caps && !caps.tsan}>
            TSan (threads)
          </option>
        </select>
        <label className="flex cursor-pointer items-center gap-1.5 rounded-md border border-zinc-300 px-2 py-1.5 dark:border-zinc-700" title="Count heap allocations and report leaks at exit">
          <input type="checkbox" checked={settings.trackAllocs} onChange={(e) => onChange({ ...settings, trackAllocs: e.target.checked })} />
          Track allocations
        </label>
        <button
          onClick={() => onChange({ ...settings, showStdin: !settings.showStdin })}
          className={`rounded-md border px-2 py-1.5 ${settings.showStdin || settings.stdin ? "border-sky-500 text-sky-600 dark:text-sky-400" : "border-zinc-300 dark:border-zinc-700"}`}
        >
          stdin{settings.stdin ? " •" : ""}
        </button>
        {extra}
      </div>
      {settings.showStdin && (
        <textarea
          value={settings.stdin}
          onChange={(e) => onChange({ ...settings, stdin: e.target.value })}
          placeholder="Input sent to the program's standard input (std::cin)"
          rows={3}
          className="w-full rounded-md border border-zinc-300 bg-white p-2 font-mono text-sm dark:border-zinc-700 dark:bg-zinc-900"
        />
      )}
    </div>
  );
}
