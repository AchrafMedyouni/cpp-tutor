---
title: "Move semantics"
slides: "168–173"
---

## Rvalue references, for real this time

Chapter 3 previewed `T&&`. Here is what binds to what (slide 168):

```cpp
int        g{ 8 };
// int &   rg{ 8 };        // ✗ an lvalue reference can't bind to an rvalue
const int & crg{ 8 };      // ✓ a const one can
int &&      rd{ 8 };       // ✓ an rvalue reference binds to rvalues...
int i{ 3 }, j{ 7 };
int && k4{ i + j };        // ✓ ...like this temporary
// int && bad{ i };        // ✗ ...but NOT to lvalues
```

A function taking `T&&` (slide 169) receives **a temporary that will be destroyed right after the call**, so it may *steal* its contents. Inside the function, the parameter itself has a name, which makes it an lvalue again.

::example{id="rvalue-refs"}

## Copy vs move: `swap`

The classic `swap` (slide 170) costs **1 copy construction + 2 copy assignments**. For a `Tableau` of a million doubles, that's three allocations and three million element copies just to exchange two objects. Yet `t1`, `t2` and `tmp` each only need to *hand over* their buffer.

**`std::move( x )`** casts the lvalue `x` to an rvalue reference (an *xvalue*). It doesn't move anything by itself. It says "you may steal from `x`". Afterwards `x` is in a **valid but unspecified** state, and you should only **destroy it or assign to it** (slide 171).

```cpp
template< typename T > void swap( T & t1, T & t2 ) {
  T tmp{ std::move( t1 ) };   // move construction
  t1 = std::move( t2 );       // move assignment
  t2 = std::move( tmp );
}
```

## Move operations for `Tableau`

Copy ≠ move (slide 172). A class gets two new members:

```cpp
Tableau( Tableau && tab );               // move constructor
Tableau & operator=( Tableau && tab );   // move assignment
```

```cpp
Tableau::Tableau( Tableau && tab ) : taille_{ tab.taille_ }, tab_{ tab.tab_ } {
  tab.tab_ = nullptr;                    // the source must stay destructible: delete[] nullptr is a no-op
}
Tableau & Tableau::operator=( Tableau && tab ) { swap( tab ); return *this; }   // our old buffer dies with tab
```

The **rule of five**: a resource-owning class defines (or `= delete`s) the destructor, both copy operations and both move operations. Better still is the **rule of zero**: compose your class from types that already manage themselves (`std::vector`, `std::unique_ptr`), and write none of the five.

::example{id="swap-copy-vs-move"}

::example{id="tableau-move"}

:::course
Slide 172 notes: "only define move assignment if the copy assignment takes its argument by const reference". With copy-and-swap **by value** (`Tableau & operator=( Tableau tab )`), that single operator already handles both cases: the parameter is copy- or move-constructed depending on the argument.
:::

:::warning
Mark move operations **`noexcept`** when they can't throw. `std::vector` only moves elements during reallocation if their move constructor is `noexcept`; otherwise it copies them, to keep its strong exception guarantee. The example shows the difference.
:::
