import type { GradeResponse } from "../lib/api";

export function TestResults({ result }: { result: GradeResponse }) {
  return (
    <div
      className={`rounded-lg border p-3 ${result.passed ? "border-emerald-400 bg-emerald-50 dark:border-emerald-800 dark:bg-emerald-950/30" : "border-zinc-200 bg-zinc-50 dark:border-zinc-800 dark:bg-zinc-900/60"}`}
    >
      <div className={`mb-2 font-semibold ${result.passed ? "text-emerald-700 dark:text-emerald-400" : ""}`}>
        {result.passed ? "🎉 " : ""}
        {result.summary}
      </div>
      <ul className="space-y-1.5 text-sm">
        {result.tests.map((t, i) => (
          <li key={i} className="flex gap-2">
            <span className={t.passed ? "text-emerald-600" : "text-red-600"}>{t.passed ? "✓" : "✗"}</span>
            <div className="min-w-0">
              <div className={t.passed ? "" : "font-medium"}>{t.name}</div>
              {t.message && (
                <pre className="mt-0.5 overflow-x-auto whitespace-pre-wrap font-mono text-xs text-red-700 dark:text-red-300">{t.message}</pre>
              )}
            </div>
          </li>
        ))}
      </ul>
    </div>
  );
}
