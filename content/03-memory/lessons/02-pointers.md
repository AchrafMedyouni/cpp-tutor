---
title: "Pointers"
slides: "59–62"
---

`&` applied to a variable gives its **address**, i.e. a pointer to it. `*` in a declaration declares a **pointer variable**. `*` applied to a pointer **dereferences** it: it gives you the object it points to.

```cpp
int i{ 5 };
int *p;        // a pointer to an int (here * is not multiplication)
p = &i;        // p points to i
*p = -7;       // modifies i
// &*p is the same as p, *&i is the same as i
```

::example{id="pointer-basics"}

::visualize{id="pointers"}


- **`nullptr`** is the null pointer. No object ever has that address, and dereferencing it is illegal (UB, usually a crash).
- Pointers to pointers: `int ** q{ &p }; **q = 3;`.
- `void *` is a generic pointer, rarely useful in C++.
- **`const` placement**: read the declaration right to left.

```cpp
int       * const ptr_constant{ &i };                // the pointer can't change; *ptr can
int const *       ptr_sur_constant{ &i };            // the pointee can't be modified through it
int const * const ptr_constant_sur_constant{ &i };   // neither
```

- With a pointer to a struct, `(*pc).real` needs parentheses because `.` binds tighter than `*`. The **`->`** operator is shorthand for it: `pc->imag`.

::example{id="const-pointers"}

::example{id="null-deref"}

## Function pointers

A pointer can point to a function. The declaration syntax is notoriously awkward (the slide calls it *compliqué !*):

::example{id="function-pointer"}

:::course
Slide 62 writes `double d = ( *pf )( 3.5 );`, but `pf` returns a `long`. The conversion is implicit, so it compiles, but the variable should be `long`.
:::
