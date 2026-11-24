---
title: "Data, variables and initialization"
slides: "27–30"
---

A **data item** has three things: a *memory zone* (a sequence of bytes at some address), a *type* (how to interpret those bytes) and a *value* (the bytes read through the type). A **variable** associates a name with a data item. **Assignment** changes the data item that a variable designates.

## Value semantics: the big difference from Python

A variable can either
- **contain** its value directly, so that no other variable holds that same value; or
- hold a **link** (a reference or pointer) to a value that other variables may share.

Assignment copies the **value** in the first case and the **link** in the second. Python gives you no choice: every name is a link to an object, so `b = a` makes `b` share `a`'s list. Most languages work like that. **C and C++ let you choose**, and the default is the first kind: `b = a` copies the whole object.

::example{id="value-semantics"}

## Defining variables

```cpp
int i;               // defined, NOT initialised: reading it is undefined behaviour (avoid)
int j{ 5 };          // ✓ explicit type, brace initialisation
int k{};             // ✓ value-initialised: 0
auto l{ 2021ul };    // ✓ type inferred from the initialiser: unsigned long
auto f = -2.129e-3f; // ✓ also fine: float
char c = 'c';        // historical (C) syntax
double d( 6.666 );   // historical (constructor) syntax
auto v = { 1 };      // ✗ avoid: v is a std::initializer_list<int>, not an int!
```

The slides recommend `{}`, and it has a real advantage: it **refuses narrowing conversions** (losing information) at compile time.

::example{id="init-syntaxes"}

::example{id="narrowing"}

::example{id="uninitialized"}

## Declarations vs definitions

A **definition** reserves the memory. A **declaration** only announces that a name exists, with its type. Every definition is also a declaration. The rules (slide 30):
- a variable must be **declared before it is used**;
- it must be **defined exactly once** in the whole program (the *one-definition rule*).

For a global variable shared between files, put an `extern` declaration in the header. `extern` declarations can't have an initialiser. Then define the variable in exactly one `.cpp`:

::example{id="extern-global"}

:::modern
Mutable globals are rarely a good idea: any function anywhere can change them. In real code prefer `const`/`constexpr` globals (next lessons) or pass values explicitly.
:::
