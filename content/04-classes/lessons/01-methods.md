---
title: "From functions to methods"
slides: "83–91"
---

With an aggregate, behaviour lives in free functions:

```cpp
struct Complexe { double real; double imag; };
double module( Complexe c ) { return std::sqrt( c.real * c.real + c.imag * c.imag ); }
```

`module` **depends on the representation** of `Complexe`, but nothing in the syntax links the two. Change the struct (say, to polar coordinates) and you must go and find every such function. The slides call this a factor of fragility, of **non-quality**.

A **class** binds the representation (*attributes*) to the services the entity offers (**methods**, or *member functions* in Stroustrup's terms). The reasoning changes from "a function receives a complex and computes its modulus" to "**a complex offers a modulus service**".

```cpp
struct Complexe {
  double real;
  double imag;
  double module() const;             // declaration, in the header
};
double Complexe::module() const {    // definition, in the .cpp: note the Complexe:: prefix
  return std::sqrt( real * real + imag * imag );
}
```

`Complexe` is now a **class**, and its values are **objects**, **instances** of the class. A method is called on a particular object with `.`: `c.module()`.

## `this`, the current object

Inside a method, the object the method was called on (the **current object** or **receiver**) is reachable through **`this`**, a pointer to it. `real` is shorthand for `this->real`. Python makes this parameter explicit as `self`. C++ makes it implicit.

## `const` methods

`module()` only *observes* the object: it is an observer, or *getter*. Mark it `const` in both the declaration and the definition. Inside it, `this` becomes a pointer to a **const** object, so the compiler rejects any modification. Only const methods can be called on a `const` object or through a `const&`, so forgetting `const` makes your class unusable in idiomatic code.

## Inline methods

A method **defined inside the class body** is implicitly `inline`: the compiler may paste its body at each call site, avoiding the call overhead. `inline` can also be written explicitly, for methods and for ordinary functions. The definition must then live in the header.

::example{id="complexe-methods"}

::example{id="const-method"}
