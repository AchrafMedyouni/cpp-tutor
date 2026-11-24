---
title: "Control structures"
slides: "37–41"
---

The general rules (slide 37):
- the condition goes **in parentheses**: `if (cond)`;
- the dependent statements go **in a block** `{ }`; the block is optional for a single statement (but consider always using one);
- newlines and indentation are ignored.

## `if` and `if … else`

```cpp
if (nombre < 0) positif = false;          // conditional
if (nombre < 0) { nombre = -nombre; positif = false; }
else           { positif = true; }        // alternative
```

## `while` and `do … while`

`while` tests **before** each iteration. `do … while` runs the body **at least once**, which makes it the natural loop for "ask until the input is valid". `break` leaves the innermost loop and `continue` jumps to the next iteration.

::example{id="loops"}

## `for`

`for (init; condition; next) body`. The three parts are optional; only the two `;` are mandatory. `for (;;)` is an infinite loop. Slide 40 uses one to print the **Collatz sequence**:

::example{id="collatz"}

## `switch`

`switch` jumps to the `case` matching an **integral** value. Without `break`, execution **falls through** into the next case. C inherited this, and it is a classic bug source.

::example{id="switch-month"}

:::course
Slide 41 declares `std::string mois;` but then assigns `m = "janvier";`, where `m` is the integer being switched on. It should be `mois = "janvier";`.
:::

::example{id="fallthrough"}
