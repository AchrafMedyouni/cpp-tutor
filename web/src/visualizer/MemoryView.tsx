import type { VizStep, VizVar } from "../lib/api";

const ROW = 28;
const HEAD = 26;
const GAP = 16;
const STACK_X = 16;
const STACK_W = 300;
const HEAP_X = 430;
const HEAP_W = 300;
const NAME_W = 118;

interface Box { x: number; y: number; w: number; h: number }
interface Anchor { left: Box; state: "live" | "freed" | "leaked" | "stack" }

/** Lay out frames and heap blocks; return their rows and an id → anchor map for arrows. */
function layout(step: VizStep) {
  const anchors = new Map<string, Anchor>();
  const frames: { name: string; box: Box; top: boolean; dead: boolean; rows: { v: VizVar; box: Box }[] }[] = [];
  let y = 34;
  step.stack.forEach((f, fi) => {
    const box = { x: STACK_X, y, w: STACK_W, h: HEAD + Math.max(f.vars.length, 1) * ROW + 6 };
    const rows = f.vars.map((v, i) => {
      const r = { x: STACK_X + 8, y: y + HEAD + i * ROW, w: STACK_W - 16, h: ROW - 4 };
      if (v.id) anchors.set(v.id, { left: { x: r.x + NAME_W, y: r.y, w: r.w - NAME_W, h: r.h }, state: f.dead ? "freed" : "stack" });
      return { v, box: r };
    });
    frames.push({ name: f.frame, box, top: fi === step.stack.length - 1 && !f.dead, dead: !!f.dead, rows });
    y += box.h + GAP;
  });
  const stackBottom = y;

  const blocks: { id: string; type: string; label?: string; state: "live" | "freed" | "leaked"; box: Box; rows: { v: VizVar; box: Box }[] }[] = [];
  y = 34;
  for (const b of step.heap) {
    const n = Math.max(b.cells.length, 1);
    const box = { x: HEAP_X, y, w: HEAP_W, h: HEAD + n * ROW + 6 };
    anchors.set(b.id, { left: box, state: b.state });
    const rows = b.cells.map((v, i) => {
      const r = { x: HEAP_X + 8, y: y + HEAD + i * ROW, w: HEAP_W - 16, h: ROW - 4 };
      if (v.id) anchors.set(v.id, { left: { x: r.x, y: r.y, w: r.w, h: r.h }, state: b.state });
      return { v, box: r };
    });
    blocks.push({ id: b.id, type: b.type, label: b.label, state: b.state, box, rows });
    y += box.h + GAP;
  }
  return { frames, blocks, anchors, height: Math.max(stackBottom, y, 140) + 10 };
}

function Value({ v, box, inHeap }: { v: VizVar; box: Box; inHeap?: boolean }) {
  const nameW = inHeap ? 70 : NAME_W;
  const isLink = v.kind !== "value";
  const shown = isLink ? (v.ref ? "" : v.value ?? "nullptr") : (v.value ?? "?");
  return (
    <g>
      <text x={box.x + 6} y={box.y + box.h / 2 + 4} className="fill-zinc-700 dark:fill-zinc-300" fontSize="12" fontFamily="ui-monospace, monospace">
        {v.name}
        {v.type && !inHeap && <tspan className="fill-zinc-400" fontSize="10">{` ${v.type}`}</tspan>}
      </text>
      <rect
        x={box.x + nameW}
        y={box.y}
        width={box.w - nameW}
        height={box.h}
        rx="4"
        className={v.highlight ? "fill-yellow-200 stroke-yellow-500 dark:fill-yellow-900/60" : "fill-white stroke-zinc-300 dark:fill-zinc-900 dark:stroke-zinc-600"}
      />
      <text x={box.x + nameW + 8} y={box.y + box.h / 2 + 4} className="fill-zinc-900 dark:fill-zinc-100" fontSize="12" fontFamily="ui-monospace, monospace">
        {shown}
      </text>
      {isLink && v.ref && <circle cx={box.x + box.w - 12} cy={box.y + box.h / 2} r="4" className={v.kind === "reference" ? "fill-violet-500" : "fill-sky-600"} />}
    </g>
  );
}

export function MemoryView({ step }: { step: VizStep }) {
  const { frames, blocks, anchors, height } = layout(step);

  // Arrows from every pointer/reference with a ref.
  const arrows: { from: { x: number; y: number }; to: { x: number; y: number }; kind: string; dangling: boolean; toHeap: boolean }[] = [];
  const addArrow = (v: VizVar, box: Box) => {
    if (v.kind === "value" || !v.ref) return;
    const t = anchors.get(v.ref);
    if (!t) return;
    const from = { x: box.x + box.w - 12, y: box.y + box.h / 2 };
    const toHeap = t.left.x >= HEAP_X;
    const to = toHeap ? { x: t.left.x, y: t.left.y + Math.min(t.left.h / 2, 14) } : { x: t.left.x + t.left.w, y: t.left.y + t.left.h / 2 };
    arrows.push({ from, to, kind: v.kind, dangling: t.state === "freed", toHeap });
  };
  frames.forEach((f) => f.rows.forEach((r) => addArrow(r.v, r.box)));
  blocks.forEach((b) => b.rows.forEach((r) => addArrow(r.v, r.box)));

  return (
    <svg viewBox={`0 0 ${HEAP_X + HEAP_W + 16} ${height}`} className="w-full select-none" role="img" aria-label="Memory diagram">
      <defs>
        <pattern id="hatch" width="6" height="6" patternUnits="userSpaceOnUse" patternTransform="rotate(45)">
          <line x1="0" y1="0" x2="0" y2="6" stroke="currentColor" strokeWidth="1.5" className="text-zinc-300 dark:text-zinc-700" />
        </pattern>
        {["sky", "violet", "red"].map((c) => (
          <marker key={c} id={`arrow-${c}`} viewBox="0 0 10 10" refX="9" refY="5" markerWidth="7" markerHeight="7" orient="auto-start-reverse">
            <path d="M 0 0 L 10 5 L 0 10 z" className={c === "sky" ? "fill-sky-600" : c === "violet" ? "fill-violet-500" : "fill-red-500"} />
          </marker>
        ))}
      </defs>
      <text x={STACK_X} y="20" fontSize="12" fontWeight="600" className="fill-zinc-500">STACK</text>
      <text x={HEAP_X} y="20" fontSize="12" fontWeight="600" className="fill-zinc-500">HEAP</text>
      {step.heap.length === 0 && (
        <text x={HEAP_X} y="52" fontSize="12" className="fill-zinc-400">(nothing allocated)</text>
      )}

      {frames.map((f, i) => (
        <g key={i}>
          <rect
            {...f.box}
            rx="8"
            fill={f.dead ? "url(#hatch)" : undefined}
            className={f.dead ? "stroke-zinc-400 dark:stroke-zinc-600" : f.top ? "fill-sky-50 stroke-sky-500 dark:fill-sky-950/40" : "fill-zinc-50 stroke-zinc-300 dark:fill-zinc-900/60 dark:stroke-zinc-700"}
            strokeWidth={f.top ? 2 : 1}
            strokeDasharray={f.dead ? "5 4" : undefined}
          />
          <text x={f.box.x + 10} y={f.box.y + 17} fontSize="12" fontWeight="600" className="fill-zinc-800 dark:fill-zinc-200" fontFamily="ui-monospace, monospace">
            {f.name}
            {f.dead && <tspan className="fill-zinc-500" fontWeight="400">{"  — returned, frame is gone"}</tspan>}
          </text>
          {f.rows.map((r, j) => <Value key={j} v={r.v} box={r.box} />)}
        </g>
      ))}

      {blocks.map((b) => (
        <g key={b.id}>
          <rect
            {...b.box}
            rx="8"
            fill={b.state === "freed" ? "url(#hatch)" : undefined}
            className={
              b.state === "leaked"
                ? "fill-red-50 stroke-red-500 dark:fill-red-950/40"
                : b.state === "freed"
                  ? "stroke-zinc-400 dark:stroke-zinc-600"
                  : "fill-amber-50 stroke-amber-400 dark:fill-amber-950/30 dark:stroke-amber-700"
            }
            strokeWidth={b.state === "leaked" ? 2.5 : 1.5}
            strokeDasharray={b.state === "freed" ? "5 4" : undefined}
          />
          <text x={b.box.x + 10} y={b.box.y + 17} fontSize="12" fontWeight="600" className="fill-zinc-800 dark:fill-zinc-200" fontFamily="ui-monospace, monospace">
            {b.type}
            <tspan className="fill-amber-700 dark:fill-amber-400" fontWeight="400">{`  #${b.id}`}</tspan>
            {b.label && <tspan className="fill-zinc-500" fontWeight="400">{`  ${b.label}`}</tspan>}
            {b.state === "freed" && <tspan className="fill-zinc-500" fontWeight="400">{"  — freed"}</tspan>}
            {b.state === "leaked" && <tspan className="fill-red-600" fontWeight="600">{"  — LEAKED"}</tspan>}
          </text>
          {b.rows.map((r, j) => <Value key={j} v={r.v} box={r.box} inHeap />)}
        </g>
      ))}

      {arrows.map((a, i) => {
        const color = a.dangling ? "red" : a.kind === "reference" ? "violet" : "sky";
        const stroke = a.dangling ? "stroke-red-500" : a.kind === "reference" ? "stroke-violet-500" : "stroke-sky-600";
        // Stack → heap: curve right. Stack → stack: loop out to the right of the frames.
        const d = a.toHeap
          ? `M ${a.from.x} ${a.from.y} C ${a.from.x + 70} ${a.from.y}, ${a.to.x - 70} ${a.to.y}, ${a.to.x - 2} ${a.to.y}`
          : `M ${a.from.x} ${a.from.y} C ${a.from.x + 60} ${a.from.y}, ${a.to.x + 60} ${a.to.y}, ${a.to.x + 2} ${a.to.y}`;
        return (
          <g key={i}>
            <path d={d} fill="none" className={stroke} strokeWidth="1.8" strokeDasharray={a.kind === "reference" ? "5 4" : undefined} markerEnd={`url(#arrow-${color})`} />
            {a.dangling && (
              <text x={(a.from.x + a.to.x) / 2} y={(a.from.y + a.to.y) / 2 - 6} fontSize="13" className="fill-red-600">⚠ dangling</text>
            )}
          </g>
        );
      })}
    </svg>
  );
}
