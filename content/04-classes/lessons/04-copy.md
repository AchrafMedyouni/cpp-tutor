---
title: "Copying objects that own resources: Tableau v2 → v4"
slides: "104–112"
---

## Tableau v2 has a copy bug

Copy construction (`Tableau tab2{ tab };`) is allowed for every class, because C structs could always be copied. The **default copy constructor copies each member**, and for `tab_` that means copying the **pointer**. Both objects then point to the **same buffer**, and at the end of the scope **both destructors delete it**: a *double destruction*.

::example{id="tableau-v2-copy"}

::visualize{id="shallow-copy"}

## The copy constructor

The **copy constructor** initialises a new object as a copy of another. It is also what the compiler uses to initialise a **by-value parameter** and a **by-value return value**. So its parameter *can't* be passed by value (`Tableau( Tableau tab )` would need to copy in order to copy, forever). It takes a **reference**, normally const:

```cpp
Tableau( const Tableau & tab );
```

The compiler-generated version copies each member using that member's own copy constructor. That's why classes built from standard components (`std::vector`, `std::string`) get **correct copies for free**.

::example{id="tableau-v3"}

:::course
Slide 107 loops with `i < tab.taille;`, which names the method without calling it. The member is `tab.taille_` (or call `tab.taille()`).
:::

## The assignment operator

`tab2 = tab;` is the other way to copy, and it is also allowed by default (member-wise). With v3 it **leaks** `tab2`'s old buffer and **shares** `tab`'s, which again leads to a double destruction.

::example{id="tableau-v3-assign"}

`operator=` must turn the *already-initialised* current object into a copy of the argument, **releasing its current representation**. By convention it returns the current object as an lvalue (`*this`), so that `( i = j ) = k;` works as it does for `int`. That's operator overloading, covered in detail in chapter 6.

## The copy-and-swap idiom

The recommended way to write it reuses the copy constructor and a `swap`:

```cpp
Tableau & operator=( const Tableau & tab ) {
  Tableau tmp{ tab };   // 1. copy (if this throws, *this is untouched)
  swap( tmp );          // 2. exchange representations with the copy
  return *this;
}                       // 3. tmp's destructor releases our OLD buffer
void swap( Tableau & tab ) {
  std::swap( taille_, tab.taille_ );
  std::swap( tab_   , tab.tab_ );
}
```

It is correct for self-assignment (`t = t;`), exception-safe, and short.

::example{id="tableau-v4"}

::visualize{id="copy-and-swap"}
