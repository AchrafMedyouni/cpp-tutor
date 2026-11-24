import Editor, { type OnMount } from "@monaco-editor/react";
import { useEffect, useId, useRef, useState } from "react";
import type { Diagnostic, SourceFile } from "../../../shared/api";
import { monaco } from "../lib/monaco";
import { useIsDark } from "../lib/theme";

interface Props {
  files: SourceFile[];
  onChange?: (files: SourceFile[]) => void;
  readOnlyFiles?: string[];
  readOnly?: boolean;
  diagnostics?: Diagnostic[];
  /** Height in px; if omitted, grows with the longest file (bounded). */
  height?: number;
  minLines?: number;
  maxLines?: number;
  /** Highlight this line (1-based) in the active file. */
  activeLine?: number;
  jumpTo?: { file: string; line: number; nonce: number };
}

export function CodeEditor({
  files,
  onChange,
  readOnlyFiles = [],
  readOnly,
  diagnostics = [],
  height,
  minLines = 6,
  maxLines = 32,
  activeLine,
  jumpTo,
}: Props) {
  const [active, setActive] = useState(files[0]?.name ?? "");
  const [mounted, setMounted] = useState(false);
  const editorRef = useRef<Parameters<OnMount>[0] | null>(null);
  const decorations = useRef<string[]>([]);
  const dark = useIsDark();
  // Each editor instance gets its own model namespace, otherwise two examples with a main.cpp share one model.
  const uid = useId().replace(/[^a-zA-Z0-9]/g, "");
  const current = files.find((f) => f.name === active) ?? files[0];

  useEffect(() => {
    if (!files.some((f) => f.name === active) && files[0]) setActive(files[0].name);
  }, [files, active]);

  // Jump to a diagnostic location.
  useEffect(() => {
    if (!jumpTo) return;
    if (jumpTo.file !== active && files.some((f) => f.name === jumpTo.file)) setActive(jumpTo.file);
    const t = setTimeout(() => {
      const ed = editorRef.current;
      if (!ed) return;
      ed.revealLineInCenter(jumpTo.line);
      ed.setPosition({ lineNumber: jumpTo.line, column: 1 });
      ed.focus();
    }, 50);
    return () => clearTimeout(t);
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [jumpTo?.nonce]);

  // Diagnostics -> markers on every model.
  useEffect(() => {
    for (const f of files) {
      const model = monaco.editor.getModels().find((m) => m.uri.path === `/${uid}/${f.name}`);
      if (!model) continue;
      const markers = diagnostics
        .filter((d) => d.file === f.name && d.severity !== "note")
        .map((d) => ({
          startLineNumber: d.line,
          startColumn: d.col,
          endLineNumber: d.line,
          endColumn: Math.max(d.col + 1, model.getLineMaxColumn(Math.min(d.line, model.getLineCount()))),
          message: d.message + (d.flag ? ` [${d.flag}]` : ""),
          severity: d.severity === "error" ? monaco.MarkerSeverity.Error : monaco.MarkerSeverity.Warning,
        }));
      monaco.editor.setModelMarkers(model, "cpplab", markers);
    }
  }, [diagnostics, files, active]);

  // Active-line decoration (visualizer).
  useEffect(() => {
    const ed = editorRef.current;
    if (!ed) return;
    decorations.current = ed.deltaDecorations(
      decorations.current,
      activeLine
        ? [
            {
              range: new monaco.Range(activeLine, 1, activeLine, 1),
              options: { isWholeLine: true, className: "monaco-line-active", linesDecorationsClassName: "monaco-glyph-active" },
            },
          ]
        : [],
    );
    if (activeLine) ed.revealLineInCenterIfOutsideViewport(activeLine);
  }, [activeLine, current?.name, mounted]);

  const lines = Math.max(...files.map((f) => f.content.split("\n").length), minLines);
  const h = height ?? Math.min(Math.max(lines, minLines), maxLines) * 19 + 16;
  const fileReadOnly = readOnly || (current && readOnlyFiles.includes(current.name));

  return (
    <div className="overflow-hidden rounded-lg border border-zinc-200 dark:border-zinc-800">
      {files.length > 1 && (
        <div className="flex gap-0.5 border-b border-zinc-200 bg-zinc-50 px-1 pt-1 text-xs dark:border-zinc-800 dark:bg-zinc-900">
          {files.map((f) => {
            const errs = diagnostics.filter((d) => d.file === f.name && d.severity === "error").length;
            return (
              <button
                key={f.name}
                onClick={() => setActive(f.name)}
                className={`rounded-t px-3 py-1.5 font-mono ${
                  f.name === current?.name
                    ? "bg-white text-zinc-900 dark:bg-zinc-950 dark:text-zinc-100"
                    : "text-zinc-500 hover:text-zinc-800 dark:hover:text-zinc-200"
                }`}
              >
                {f.name}
                {readOnlyFiles.includes(f.name) && <span className="ml-1 opacity-50">🔒</span>}
                {errs > 0 && <span className="ml-1 text-red-500">●</span>}
              </button>
            );
          })}
        </div>
      )}
      {current && (
        <Editor
          height={h}
          path={`${uid}/${current.name}`}
          language="cpp"
          theme={dark ? "vs-dark" : "vs"}
          value={current.content}
          onMount={(ed) => {
            editorRef.current = ed;
            setMounted(true);
          }}
          onChange={(v) => {
            if (fileReadOnly || !onChange) return;
            onChange(files.map((f) => (f.name === current.name ? { ...f, content: v ?? "" } : f)));
          }}
          options={{
            readOnly: !!fileReadOnly,
            minimap: { enabled: false },
            fontSize: 13,
            lineNumbersMinChars: 3,
            scrollBeyondLastLine: false,
            automaticLayout: true,
            tabSize: 2,
            renderLineHighlight: readOnly ? "none" : "line",
            scrollbar: { alwaysConsumeMouseWheel: false },
            padding: { top: 8, bottom: 8 },
          }}
        />
      )}
    </div>
  );
}
