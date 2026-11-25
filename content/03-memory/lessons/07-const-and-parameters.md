---
title: "Cheat sheet: const, pointers, references and parameters"
slides: "59–75"
---

This lesson adds no new slides. It gathers the rules of the previous lessons into **one grid and three rules**, the ones every "does this compile?" question comes down to. Then practise them in the [⚡ drill](/ch/03-memory/drill) until they're automatic.

## 1. `const` with pointers: read right to left

Start at the variable's name and read **leftwards**:

| declaration | read as | `*p = 5` | `p = &other` |
|---|---|:---:|:---:|
| `int* p` | p is a pointer to int | ✅ | ✅ |
| `const int* p` (= `int const* p`) | p is a pointer to a **const int** | ❌ | ✅ |
| `int* const p` | p is a **const pointer** to int | ✅ | ❌ |
| `const int* const p` | p is a const pointer to a const int | ❌ | ❌ |

- `const` **left of `*`**: the **pointee** is read-only *through this pointer*. It's a read-only **view**: the object itself may still change by other paths.
- `const` **right of `*`**: the **pointer** can't be re-aimed (and so must be initialised).
- A reference can never be re-aimed anyway, so it only has the first kind: `const int& r` is a read-only alias.

::example{id="const-right-to-left"}

## 2. Which arguments does a parameter accept?

With `int x;` (a variable), `const int cx;` and a temporary (`42`, `x + 1`, `f()`):

| parameter ↓ / argument → | `x` | `cx` | temporary |
|---|:---:|:---:|:---:|
| `int` (by value) | ✅ copy | ✅ copy | ✅ copy |
| `int&` | ✅ | ❌ would drop const | ❌ |
| `const int&` | ✅ | ✅ | ✅ special rule |
| `int*`, called with `&x` / `&cx` | ✅ | ❌ would drop const | ❌ no address |
| `const int*`, called with `&x` / `&cx` | ✅ | ✅ | ❌ no address |
| `int&&` (chapter 7) | ❌ | ❌ | ✅ |

Everything in this grid follows from two rules:

1. **You can add `const`, never remove it.** `int*` → `const int*` and `int&` → `const int&` are implicit. The other direction would let someone modify a const object, so the compiler refuses.
2. **Temporaries bind only to a by-value parameter, `const T&` or `T&&`.** Modifying a temporary through `T&` would be pointless, so it's forbidden. `const T&` is allowed, and the temporary lives until the end of the call.

:::warning
The same rules apply **inside** the function and on the **return type**. If you received something as `const T&` or `const T*`, you can only hand it back as `const T&` or `const T*`. That was the `best()` function in the *Fix the const-correctness* exercise.
:::

## 3. Choosing a parameter type

| the function… | use | example |
|---|---|---|
| must **modify** the caller's object | `T&` | `void scale(Position& p, double k)` |
| …or modifies it, and "no object" is a valid input | `T*` (check for `nullptr`) | `void reset(Position* p)` |
| only **reads** a big object (string, vector, class) | `const T&` | `double notional(const Position& p)` |
| only reads a small value (`int`, `double`, a 2-field struct) | `T` by value | `double square(double x)` |
| wants its own copy anyway | `T` by value | `std::string upper(std::string s)` |

::example{id="parameter-passing"}

::visualize{id="pass-by"}

:::note
**For Python programmers.** In Python, every argument is passed the same way, and whether the function can modify it depends on the **object** (mutable list or immutable int). In C++ the **signature** decides. `T` gives the function a private copy, `T&` gives it write access to the caller's object, and `const T&` gives it read-only access with no copy. A C++ reference also differs from a Python name in one way: it can never be re-bound to another object.
:::

## 4. Function pointers: read from the name outwards

`double (*f)(double)` reads: `f` is a **pointer** (`*f`) to a function taking a `double` and returning a `double`. The parentheses are mandatory: `double* f(double)` declares a *function* returning `double*`. In practice:

```cpp
using Payoff = double (*)(double);          // an alias makes it readable
Payoff f = square;                          // a function name decays to a pointer
std::function<double(double)> g = [k](double s) { return k * s; };   // anything callable, even capturing lambdas
```

:::tip
Now drill it: [**⚡ Signatures & const drill**](/ch/03-memory/drill) has about 70 rapid-fire "does it compile?" cards in four sections that follow this page. Every answer is checked by the real compiler, and after each card you can see the exact error message.
:::
