---
title: "The type of a function"
slides: "49–50"
---

`long partie_entiere( double d );` has type **`long( double )`**. A variable can't *contain* a function, but it can hold a **link** to one. Because there are several kinds of links (function pointers, lambdas, objects with `operator()`), the standard library provides a general wrapper, **`std::function`** (header `<functional>`):

```cpp
std::function< long( double ) > foo;   // can hold anything callable as long(double)
foo = partie_entiere;
auto l = foo( 2.5 );
void bar( std::function< long( double ) > f );   // as a parameter
bar( partie_entiere );
```

Passing behaviour as a parameter gives you higher-order functions, as in Python (`sorted(key=...)`). Here is a classic numerical example: finding the yield that prices a bond, by **bisection** on any function.

::example{id="std-function"}

::example{id="bisection"}
