---
title: Input and output with streams
slides: "25"
---

I/O goes through **stream objects** from `<iostream>`. Unlike `print`/`input` in Python, these are not built into the language:

| Object | Meaning |
|---|---|
| `std::cin` | standard input |
| `std::cout` | standard output |
| `std::cerr` | standard error (unbuffered) |

`<<` and `>>` are ordinary C++ operators that the library **overloads**, so `std::cout << x` calls a function chosen by the type of `x`. Because each call returns the stream, calls can be **chained**: `std::cout << a << b` is `(std::cout << a) << b`. Chapter 6 shows how to make `<<` work for your own types (*extensible to user types*).

::example{id="square"}

`std::cin >> n` skips whitespace, then parses an `int`. If parsing fails (input `abc`, or end of input), the stream enters a **fail state**: `n` is set to 0 (since C++11), and **every later read does nothing** until you call `std::cin.clear()`. Always check a read before using the value.

:::course
The slide tests `std::cin.good()`. That is subtly too strict. `good()` is also false when the read **succeeded but reached the end of the input**, e.g. stdin is `12` with no final newline, which sets `eofbit`. Test the read itself instead: `if (std::cin >> n)` (equivalently `!std::cin.fail()`). The example below demonstrates the difference.
:::

The idiomatic loop reads *until the input runs out or is invalid*. `std::cin >> x` evaluates to the stream, which converts to `false` once it has failed:

::example{id="read-prices"}

:::tip
`std::endl` writes `'\n'` **and flushes** the buffer. In a loop, prefer `"\n"`, because flushing on every line is slow. Use `std::endl` (or `std::flush`) when the output must appear *now*, e.g. before a long computation.
:::

:::finance
Formatting numbers: `#include <iomanip>`, then `std::cout << std::fixed << std::setprecision(2) << 3.14159` prints `3.14`. Both settings are *sticky*: they apply to every later `<<` on that stream.
:::
