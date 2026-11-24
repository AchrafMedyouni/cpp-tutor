---
title: "Operators and their properties"
slides: "136–140"
---

C++ has a lot of operators (slide 136):

| arithmetic | inc/dec | assignment | comparison | logical | access | other |
|---|---|---|---|---|---|---|
| `+a -a a+b a-b a*b a/b a%b ~a a&b a\|b a^b a<<b a>>b` | `++a --a a++ a--` | `= += -= *= /= %= &= \|= ^= <<= >>=` | `== != < > <= >= <=>` | `!a a&&b a\|\|b` | `a[b] *a &a a->b a.b a->*b a.*b` | `a(...) a,b a?b:c` |

There are also the keyword operators (`static_cast` and the other casts, `sizeof`, `typeid`, `new`, `delete`, `noexcept`, `alignof`, `co_await`) and alternative spellings (`and`, `bitand`, `and_eq` …).

Every operator has fixed **properties**, and overloading **cannot change them**:
- **arity**: the number of operands;
- **position**: prefix (`-a`), infix (`a + b`) or postfix (`a++`);
- **associativity** when the same operator is repeated: `a - b - c` is `(a - b) - c`, but `a = b = c` is `a = (b = c)`;
- **precedence** between different operators (`*` before `+`); use parentheses to override it;
- **evaluation order** of the operands: unspecified in most cases.

::example{id="eval-order"}

:::course
Slide 137 shows `tab[i] = i++;  // ???`, which was undefined behaviour before C++17. **Since C++17** the right operand of `=` is evaluated before the left one, so the line is well defined: `i++` runs first and `tab[new i]` receives the old `i`. The order of **function arguments** (`f(g(), h())`) is still unspecified.
:::

## How an overloaded operator is called

Writing `a ∆ b` makes the compiler look for **either**
- a **free function** `operator∆( a, b )`, or
- a **member function** `a.operator∆( b )`.

A unary operator `∆ a` becomes `operator∆( a )` or `a.operator∆()`. The operator really is just a function with a funny name, and you can even call it by that name:

::example{id="operator-syntax"}

Operators that can be overloaded (slides 139–140): all of the arithmetic, bitwise, comparison, logical, assignment and increment operators, plus `[]`, `->`, `->*`, unary `&` and `*`, `new`/`delete` (and the `[]` forms), `()` (call, any arity) and `,`. You **can't** overload `.`, `::`, `?:`, `sizeof` or the casts, and you can't invent new operators.

:::warning
Overloading `&&`, `||` or `,` loses their short-circuit or sequencing behaviour, because the overloads are ordinary function calls whose arguments are all evaluated. Don't overload them.
:::
