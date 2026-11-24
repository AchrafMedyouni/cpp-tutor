---
title: "Functions"
slides: "42–48"
---

## Definition and declaration

```cpp
double carre( double d ) { return d * d; }           // usual syntax (from C)
auto carre( double d ) -> double { return d * d; }   // trailing return type
extern double carre( double d );                     // declaration (extern optional)
auto carre( double d ) -> double;                    // declaration, trailing style
```

Functions follow the same rules as variables: **declared before use, defined exactly once**.

## Passing arguments and returning values by value

By default an argument is **passed by value**: the parameter is a **copy** of the argument, so modifying it doesn't affect the caller. Any expression can be passed, e.g. `f1(5)` or `f1(j + 2)`. Return values are also copies of the `return` expression. Chapter 3 adds passing by address and by reference.

::example{id="pass-by-value"}

## Signature and overloading

The **signature** of `long partie_entiere( double d );` is `partie_entiere(double)`: the name plus the parameter types. Parameter names and the return type are **not** part of it. Two functions can share a name as long as their signatures differ (**overloading**). The compiler picks the best match for the argument types. Python can't do this; it would use `isinstance` or `functools.singledispatch`.

::example{id="overloading"}

## Default values

Parameters can have default values. Once a parameter has one, **all parameters after it** must have one too.

::example{id="default-args"}
