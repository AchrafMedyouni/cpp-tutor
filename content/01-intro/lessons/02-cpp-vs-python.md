---
title: C++ for Python programmers
slides: "19"
---

Five syntactic differences matter from day one:

1. **Static typing.** Every variable, parameter and return value has a type fixed at compile time. Assigning an incompatible value is a *compile* error, not a runtime one. (`auto` still gives you type *inference*, covered in chapter 2.)
2. **Lines don't matter, `;` does.** A statement ends with `;`. One statement can span several lines, and one line can hold several statements.
3. **Indentation is ignored.** Blocks are delimited by `{ }` and can be nested. Indent anyway: humans read the code too.
4. **Newline is just whitespace.** It works as a *separator*, exactly like a space or a tab.
5. **Two comment styles:** `/* … */` runs until the first `*/` (it can span lines but not nest), and `//` runs to the end of the line.

::example{id="python-vs-cpp"}

Every program starts in `int main()`. Its return value is the process exit code: `0` means success. If `main` reaches its closing brace without a `return`, it returns 0 (this applies only to `main`).

:::warning
A missing `;` after a `}` that ends a `struct`/`class` definition is the classic first C++ error. Function bodies don't take one. Class definitions do: `struct S { int x; };`.
:::
