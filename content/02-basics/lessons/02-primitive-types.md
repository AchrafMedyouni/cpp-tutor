---
title: "Primitive types"
slides: "31"
---

| Family | Types (size guaranteed only as an ordering) | Literals |
|---|---|---|
| integers | `char` ≤ `short` ≤ `int` ≤ `long` ≤ `long long`, each `signed` or `unsigned` | `25`, `62L`, `0xFE23U`, `015UL` (octal!), `-53LL`, `0b10010011` (C++14) |
| floating point | `float` ≤ `double` ≤ `long double` | `3.14159`, `5.28E-12F`, `1.L` |
| boolean | `bool` | `false`, `true` |
| characters | `char`, `wchar_t`, `char8_t`, `char16_t`, `char32_t` | `'a'`, `'\t'` |

Unlike Python's `int`, C++ integers have a **fixed width**. On today's 64-bit machines `int` is usually 32 bits, `long` 64 bits on Linux/macOS (but 32 bits on Windows) and `long long` 64 bits. When the width matters, use `<cstdint>`: `std::int64_t`, `std::uint32_t`.

C has an old rule: `0` is `false` and any other value is `true`. That's why `if (n)` works on integers and pointers.

::example{id="type-sizes"}

## Overflow: unsigned wraps, signed is undefined

Unsigned arithmetic is **modulo 2ⁿ**: it wraps around, and that is well defined. **Signed overflow is undefined behaviour**: the compiler may assume it never happens, and the program can do anything. UBSan catches it at run time.

::example{id="signed-overflow"}

## Floating point is not decimal

`double` is binary floating point, so `0.1` can't be represented exactly. Never compare doubles with `==` after arithmetic, and never store money in a `double` (chapter 6 builds a `Money` type in integer cents).

::example{id="float-precision"}
