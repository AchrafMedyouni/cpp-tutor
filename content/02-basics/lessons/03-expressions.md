---
title: "Expressions and operators"
slides: "32–34"
---

Operators work much as in Python, with a few differences:

- `=` assigns. **Assignment is an expression** whose value is the assigned variable, so `a = b = 0;` works, and so does the classic bug `if (x = 0)`.
- Arithmetic: `+ - * / %`. **`/` between two integers is integer division**, truncated **towards zero** (`-7 / 2 == -3`, whereas Python's `//` gives `-4`). There is **no `**` operator**: use `std::pow` from `<cmath>`.
- `* / %` bind tighter than `+ -`. Use parentheses for anything else.
- Bitwise operators: `&` (and), `|` (or), `^` (xor), `<<`, `>>` (shifts) and `~` (one's complement, unary).
- Comparisons: `== != < <= > >=`. Logical operators: `&&`, `||`, `!` (`and`, `or`, `not` are also accepted). `&&` and `||` **short-circuit**: the right operand isn't evaluated if the left one decides the result.
- Compound assignment: `i += 2` means `i = i + 2`, and the same exists for every binary operator.
- Increment and decrement: `++i` increments, then yields the new value. `i++` yields the old value, then increments.

::example{id="int-division"}

::example{id="increments"}

The **conditional operator** `?:` is the expression form of `if … else` (Python's `a if cond else b`):

::example{id="ternary-parity"}

To force a floating-point division between two integers, **convert one of them explicitly** with `static_cast<double>(...)`:

::example{id="static-cast"}

:::course
Slide 34 writes `double d{ static_cast< double >( i ) / j;`, which is missing the closing `}`. The intended line is `double d{ static_cast<double>(i) / j };`.
:::

::example{id="order-flags"}
