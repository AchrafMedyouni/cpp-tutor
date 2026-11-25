import type { IoTest, SourceFile } from "../lib/api";
import { inlineMd } from "./ExampleRunner";
import { CodeEditor } from "./CodeEditor";

/** Explanations shown only for the constructs a given test file actually uses. */
const LEGEND: [RegExp, string][] = [
  [/\bTEST_CASE\b/, "`TEST_CASE(\"name\")`: one test. Its name is what appears in the results after *Check*."],
  [/\bCHECK\(/, "`CHECK(expr)`: `expr` must be true. The test continues even if it fails."],
  [/\bREQUIRE\(/, "`REQUIRE(expr)`: like `CHECK`, but stops this test case on failure (used when later lines would crash)."],
  [/\bCHECK_FALSE\(/, "`CHECK_FALSE(expr)`: `expr` must be false."],
  [/\bCHECK_THROWS_AS\(/, "`CHECK_THROWS_AS(expr, E)`: evaluating `expr` must throw an exception of type `E`."],
  [/\bCHECK_NOTHROW\(/, "`CHECK_NOTHROW(expr)`: evaluating `expr` must not throw."],
  [/doctest::Approx/, "`x == doctest::Approx(y)`: equal up to floating-point rounding."],
  [/live_allocations/, "`cpplab::live_allocations()`: number of heap blocks currently allocated (`new` not yet `delete`d). Comparing it before/after a scope detects **leaks**."],
  [/total_allocations/, "`cpplab::total_allocations()`: number of `new` calls so far. Unchanged before/after means **no allocation happened** (e.g. a move, not a copy)."],
  [/Tracked/, "`cpplab::Tracked`: a type that counts how often it is constructed, copied, moved and destroyed (`Tracked::counts.copy_ctor`, `.move_ctor`, `.dtor`…)."],
  [/std::is_\w+_v/, "`std::is_..._v<T>` (from `<type_traits>`): compile-time facts about a type, e.g. `std::is_copy_constructible_v<T>` is true if `T` can be copied, `std::is_convertible_v<A, B>` if an `A` converts *implicitly* to a `B`."],
  [/static_assert/, "`static_assert(cond)`: checked at **compile time**; if false, nothing compiles."],
];

export function TestsPanel({
  tests,
  io,
  note,
  defaultOpen = true,
}: {
  tests?: SourceFile[];
  io?: IoTest[];
  note?: string;
  defaultOpen?: boolean;
}) {
  const hasTests = !!tests?.length;
  const hasIo = !!io?.length;
  if (!hasTests && !hasIo) return null;
  const all = tests?.map((t) => t.content).join("\n") ?? "";
  const legend = LEGEND.filter(([re]) => re.test(all)).map(([, text]) => text);
  const count = hasTests ? (all.match(/\bTEST_CASE\s*\(/g) ?? []).length : io!.length;

  return (
    <details open={defaultOpen} className="group rounded-lg border border-sky-200 bg-sky-50/40 dark:border-sky-900 dark:bg-sky-950/20">
      <summary className="cursor-pointer select-none px-3 py-2 text-sm font-medium">
        🧪 The tests your code must pass <span className="font-normal text-zinc-500">({count} test{count === 1 ? "" : "s"}: this is exactly what <i>Check</i> runs)</span>
      </summary>
      <div className="space-y-3 px-3 pb-3">
        {note && <p className="text-sm text-zinc-600 dark:text-zinc-400" dangerouslySetInnerHTML={{ __html: inlineMd(note) }} />}
        {hasTests && <CodeEditor files={tests!} readOnly maxLines={45} />}
        {hasIo && (
          <table className="w-full table-fixed border-collapse text-sm">
            <thead>
              <tr className="text-left text-xs text-zinc-500">
                <th className="w-1/4 pb-1 pr-2 font-medium">test</th>
                <th className="pb-1 pr-2 font-medium">stdin (what the program reads)</th>
                <th className="pb-1 font-medium">expected stdout</th>
              </tr>
            </thead>
            <tbody>
              {io!.map((t) => (
                <tr key={t.name} className="border-t border-sky-100 align-top dark:border-sky-900/60">
                  <td className="py-1.5 pr-2">{t.name}</td>
                  <td className="py-1.5 pr-2">
                    <pre className="whitespace-pre-wrap font-mono text-xs">{t.stdin || <span className="text-zinc-400">(empty)</span>}</pre>
                  </td>
                  <td className="py-1.5">
                    <pre className="whitespace-pre-wrap font-mono text-xs">{t.expectedStdout}</pre>
                    {t.match === "regex" && <div className="text-xs text-zinc-500">(regular expression)</div>}
                  </td>
                </tr>
              ))}
            </tbody>
          </table>
        )}
        {hasIo && (
          <p className="text-xs text-zinc-500">
            Your program's stdout is compared with the expected output, ignoring spaces at the end of lines and blank lines at the start/end
            {io!.some((t) => t.match === "exact") ? " (except tests marked exact)" : ""}.
          </p>
        )}
        {legend.length > 0 && (
          <details className="text-sm">
            <summary className="cursor-pointer text-xs text-zinc-500">How to read these tests</summary>
            <ul className="mt-1 list-disc space-y-0.5 pl-5 text-zinc-700 dark:text-zinc-300">
              {legend.map((l) => (
                <li key={l} dangerouslySetInnerHTML={{ __html: inlineMd(l) }} />
              ))}
            </ul>
          </details>
        )}
      </div>
    </details>
  );
}
