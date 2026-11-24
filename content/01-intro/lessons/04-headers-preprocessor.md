---
title: Headers, include guards and the preprocessor
slides: "20–22"
---

## Modules = header + implementation

C++ (before C++20 modules, which this course doesn't use) builds a *module* from two files:

| File | Contains | Included by |
|---|---|---|
| `A.hpp` (header) | **type definitions** (`struct`, `class`, `enum`) and **declarations** of services (functions, `extern` variables) | every file that uses the module |
| `A.cpp` (implementation) | **definitions** of the services | nobody, it is compiled on its own |

`A.cpp` includes `A.hpp` too, so the compiler can check that the definitions match the declarations. Python's `import` loads a module object. `#include` is much cruder: it **pastes the file's text** in place.

## The multiple-inclusion problem

Textual pasting causes trouble when headers include each other. In slide 21, `B.hpp` and `C.hpp` both include `A.hpp`, and `C.cpp` includes both of them. After preprocessing, `class A { };` appears **twice** in the same translation unit. Defining a type twice is an error.

::example{id="multiple-inclusion"}

## The fix: include guards

Wrap every header in a guard, so that its body is skipped the second time it is included:

```cpp
// File A.hpp
#ifndef A_HPP_INCLUDED      // first time: not defined yet → keep going
#define A_HPP_INCLUDED      // define it, so that the next inclusion skips everything
class A { };
#endif
```

`#pragma once` does the same in one line. It isn't in the ISO standard, but every mainstream compiler supports it. **Rule: every header gets a guard, always.**

::example{id="include-guards"}

## Other preprocessor uses

- `#include <iostream>` (angle brackets) searches the system/standard directories. `#include "A.hpp"` (quotes) searches next to the current file first.
- **Conditional compilation:** `#ifdef DEBUG … #endif` keeps a block only if the macro `DEBUG` is defined, for example with `-DDEBUG` on the compiler's command line. The code is removed *before* compilation, so it costs nothing in a release build.

::example{id="debug-define"}

:::modern
The preprocessor knows nothing about C++: it substitutes text. Use it for includes, guards and conditional compilation, and for nothing else. Chapter 2 shows `constexpr` replacing `#define` constants.
:::
