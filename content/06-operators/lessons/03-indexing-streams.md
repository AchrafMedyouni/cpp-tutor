---
title: "Indexing and stream operators"
slides: "143–145"
---

## `operator[]`

`Tableau::get(i)` becomes the natural `tab[i]`. Return a **reference** so that `tab[i] = x;` works. Provide a **const overload** too, so that indexing also works on a `const Tableau&`:

```cpp
double &       operator[]( unsigned indice );         // on a non-const object: read and write
const double & operator[]( unsigned indice ) const;   // on a const object: read only
```

::example{id="tableau-index"}

## Extending `<<` and `>>` to your types

`std::cout << c` calls `operator<<( std::cout, c )`. You can't add members to `std::ostream`, so these are **free functions**. Each returns the stream by reference so that calls chain (slide 145):

```cpp
std::ostream & operator<<( std::ostream & out, const Complexe & c );
std::istream & operator>>( std::istream & in, Complexe & c );   // non-const c: it is written to
```

On a parse error, `>>` should put the stream in its fail state (`in.setstate(std::ios::failbit)`) and leave the target unchanged. That keeps `while (std::cin >> c)` working for your type exactly as for `int`.

::example{id="stream-operators"}

## The call operator `()`

A class with `operator()` produces **function objects** (*functors*): objects that can be called like functions and carry state. Chapter 7 uses them with the standard algorithms, and lambdas are syntactic sugar for exactly this.

::example{id="functor"}
