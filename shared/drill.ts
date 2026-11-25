/**
 * A drill card is a one-liner judged against its section's "Given:" code.
 * The same assembly is used by verify-content (to prove each answer) and by the
 * web app ("show me what the compiler says"), so both always agree.
 */
export interface DrillCardSource {
  code: string;
  scope?: "main" | "global";
}

const QUIET = [
  "// Unused-variable noise would distract from the one error that matters.",
  '#pragma GCC diagnostic ignored "-Wunused-variable"',
  '#pragma GCC diagnostic ignored "-Wunused-but-set-variable"',
  '#pragma GCC diagnostic ignored "-Wunused-parameter"',
];

export function drillProgram(prelude: string, card: DrillCardSource): string {
  const global = card.scope === "global";
  return [
    ...QUIET,
    "",
    "// ---- Given ----",
    prelude.trimEnd(),
    "",
    ...(global ? ["// ---- Card ----", card.code.trimEnd(), "", "int main() {}"] : ["int main() {", "  // ---- Card ----", ...card.code.trimEnd().split("\n").map((l) => "  " + l), "}"]),
    "",
  ].join("\n");
}
