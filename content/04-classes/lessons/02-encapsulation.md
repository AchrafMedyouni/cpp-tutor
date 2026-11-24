---
title: "Access control, constructors, accessors"
slides: "92–97"
---

## `private` and `public`

The method gives `Complexe` a modulus service, but nothing stops client code from touching `real` and `imag` directly, which is still fragile. Declare the attributes **private** and keep the services **public**:

```cpp
struct Complexe {
private:
  double real;
  double imag;
public:
  double module() const;
};
```

The restriction applies to code **outside** the class. Members (like `module()`) still have full access.

## `struct` vs `class`

`class` is equivalent to `struct`. Stroustrup introduced the keyword because it is *more telling*. The **only** difference is the default access: `public` for `struct`, `private` for `class`. One common convention: `class` for objects with an *identity* and invariants, `struct` for plain bundles of values.

## Constructors

Once members are private, `Complexe c{ 2, -5 };` no longer compiles, because client code can't initialise private members. A **constructor** is a special method that initialises an object. It:
- has the **same name as the class**;
- declares **no return type** and returns nothing;
- can take 0 or more parameters, and **can be overloaded**.

Once a class has **any** constructor, every object must be created through one of them. In particular `Complexe c;` no longer compiles unless there is a constructor taking no arguments.

::example{id="constructor"}

## Name resolution

In `this->real = real;`, the unqualified `real` refers to the **parameter**: the compiler searches the innermost scope (the method) first. `this->` forces the search to start in the object. The slide's advice is simple: **don't reuse names for different things**. A common convention suffixes members with `_` (`real_`), as the next slides do.

::example{id="self-assign"}

## Accessors

To read or change `real_` from outside, add **accessors**:

```cpp
double real() const { return real_; }          // getter
void real( double value ) { real_ = value; }   // setter
```

*So what's the point of protection?* (slide 97) Two things:
1. **The representation can change** without touching client code. Store polar coordinates tomorrow and `real()` computes `r·cos θ`; every caller is unaffected.
2. **Invariants**: a setter can *refuse* invalid states, and a plain public field can't. A bank account never goes negative; an order's quantity is always positive.

::example{id="representation-change"}

::example{id="invariants"}
