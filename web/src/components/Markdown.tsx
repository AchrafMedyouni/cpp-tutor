import ReactMarkdown from "react-markdown";
import rehypeHighlight from "rehype-highlight";
import remarkDirective from "remark-directive";
import remarkGfm from "remark-gfm";
import { visit } from "unist-util-visit";
import type { Plugin } from "unified";
import { Link } from "react-router-dom";
import { ExampleRunner } from "./ExampleRunner";

/**
 * Directives supported in lesson Markdown:
 *   ::example{id="name"}            runnable example from the chapter's examples/ folder (or id="chapter/name")
 *   ::visualize{id="scenario"}      link to a memory-visualizer scenario
 *   :::note / :::course / :::modern / :::warning / :::finance   callout blocks
 */
const directivesPlugin: Plugin = () => (tree) => {
  visit(tree, (node: any) => {
    if (node.type === "leafDirective" || node.type === "containerDirective" || node.type === "textDirective") {
      const data = node.data || (node.data = {});
      if (node.name === "example" || node.name === "visualize") {
        data.hName = `cpplab-${node.name}`;
        data.hProperties = { ...node.attributes };
      } else if (["note", "course", "modern", "warning", "finance", "tip"].includes(node.name)) {
        data.hName = "aside";
        data.hProperties = { "data-callout": node.name };
      } else if (node.type === "textDirective") {
        // Not a directive we know (e.g. "a:b" in text): put the text back.
        data.hName = "span";
        node.children = [{ type: "text", value: `:${node.name}` }, ...(node.children ?? [])];
      }
    }
  });
};

const CALLOUT: Record<string, { title: string; cls: string }> = {
  note: { title: "Note", cls: "border-sky-400 bg-sky-50 dark:bg-sky-950/30" },
  tip: { title: "Tip", cls: "border-emerald-400 bg-emerald-50 dark:bg-emerald-950/30" },
  course: { title: "📑 On the slides", cls: "border-violet-400 bg-violet-50 dark:bg-violet-950/30" },
  modern: { title: "✨ Modern C++", cls: "border-emerald-400 bg-emerald-50 dark:bg-emerald-950/30" },
  warning: { title: "⚠️ Pitfall", cls: "border-amber-400 bg-amber-50 dark:bg-amber-950/30" },
  finance: { title: "📈 Finance corner", cls: "border-teal-400 bg-teal-50 dark:bg-teal-950/30" },
};

export function Markdown({ children, chapter }: { children: string; chapter: string }) {
  return (
    <div className="prose prose-zinc max-w-none dark:prose-invert prose-headings:scroll-mt-20 prose-pre:text-[13px] prose-code:text-[0.88em]">
      <ReactMarkdown
        remarkPlugins={[remarkGfm, remarkDirective, directivesPlugin]}
        rehypePlugins={[[rehypeHighlight, { detect: false, ignoreMissing: true }]]}
        components={
          {
            "cpplab-example": ({ id }: { id: string }) => {
              const [ch, ex] = id.includes("/") ? id.split("/") : [chapter, id];
              return <ExampleRunner chapter={ch} id={ex} />;
            },
            "cpplab-visualize": ({ id }: { id: string }) => (
              <Link
                to={`/visualizer/${id}`}
                className="not-prose my-4 flex items-center gap-3 rounded-lg border border-indigo-300 bg-indigo-50 px-4 py-3 text-sm no-underline hover:bg-indigo-100 dark:border-indigo-800 dark:bg-indigo-950/40 dark:hover:bg-indigo-950/70"
              >
                <span className="text-2xl">🧠</span>
                <span>
                  <b className="text-indigo-800 dark:text-indigo-300">Step through it in the memory visualizer</b>
                  <br />
                  <span className="text-zinc-600 dark:text-zinc-400">See the stack, the heap and every pointer, line by line.</span>
                </span>
              </Link>
            ),
            aside: ({ children, ...props }: any) => {
              const c = CALLOUT[props["data-callout"]] ?? CALLOUT.note;
              return (
                <aside className={`my-4 rounded-r-md border-l-4 px-4 py-1 text-[0.95em] ${c.cls}`}>
                  <div className="mt-2 text-xs font-bold uppercase tracking-wide opacity-70">{c.title}</div>
                  {children}
                </aside>
              );
            },
            a: ({ href, children }: any) =>
              href?.startsWith("/") ? <Link to={href}>{children}</Link> : <a href={href} target="_blank" rel="noreferrer">{children}</a>,
          } as any
        }
      >
        {children}
      </ReactMarkdown>
    </div>
  );
}
