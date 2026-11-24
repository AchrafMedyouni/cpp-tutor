---
title: "Aggregates and enumerations"
slides: "56–58"
---

## Aggregates: `struct`

An **aggregate** describes a data item as a list of named, typed **attributes** (members). It's the closest thing to a Python `@dataclass` without methods:

```cpp
struct Complexe {
  double real;
  double imag;
};                 // ← the semicolon is mandatory!
```

Members can have different types, but their names must be distinct. The new **user type** is then used like a primitive type: `Complexe c{ 1.3, -5.2 };` initialises the members in order. Members are accessed with `.`, for reading and for writing.

**Assignment copies every member** (value semantics again). **Comparison with `==` does not work** unless you define it. That's *operator overloading*, covered in chapter 6.

::example{id="complexe-struct"}

::example{id="struct-equality"}

## Enumerations

An **enumeration** is a list of named constants:

```cpp
enum class Color { red, green, blue };   // scoped: Color::green, no implicit conversion to int
Color c{ Color::green };
```

By default the first constant converts (explicitly, with `static_cast<int>`) to 0 and each following one to the previous value + 1. You can impose values, and they don't have to be distinct. The old C enums (`enum Niveau { nul, moyen = 10, bon = 15 };`) are less strict: their names leak into the surrounding scope (`moyen`, not `Niveau::moyen`) and they **convert implicitly** to `int`. Prefer `enum class`.

::example{id="enums"}
