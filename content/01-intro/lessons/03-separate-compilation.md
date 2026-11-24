---
title: Separate compilation and linking
slides: "17–18"
---

A C++ program is usually spread over many `.cpp` files. Building it has three stages:

```
          preprocess + compile           link
A.cpp  ─────────────────────────▶  A.o ──┐
B.cpp  ─────────────────────────▶  B.o ──┼──▶  a.out  (executable)
                                 lib.a ──┘
```

1. The **preprocessor** expands `#include`, `#define` and `#if` *textually*. The result of one `.cpp` after preprocessing is called a **translation unit**.
2. The **compiler** turns each translation unit into an **object file** (`.o`): machine code with *holes* for every function or variable defined elsewhere.
3. The **linker** (*éditeur de liens*) fills the holes by matching names across the object files and libraries, and produces the executable.

Each `.cpp` is compiled **independently**, so one file cannot "see" another file's code. To call a function defined in `B.cpp`, `A.cpp` only needs its **declaration** (its signature). The linker finds the **definition** later. CppLab does exactly this: every `.cpp` tab is compiled to its own `.o`, then everything is linked.

::example{id="two-files"}

When the linker can't find a definition you get a *link* error. The compiler was happy, because the declaration said the function exists somewhere:

::example{id="link-error"}

:::tip
Read error messages for the **stage** they come from. `error: use of undeclared identifier` comes from the compiler: you forgot a declaration or an `#include`. `Undefined symbols … / undefined reference to` comes from the linker: you declared something but never defined it, or forgot to compile and link the file that defines it.
:::

Why bother? On a big codebase, editing one `.cpp` only recompiles that file, and linking is fast. Python re-reads modules at startup instead. C++ trades build complexity for zero runtime cost.
