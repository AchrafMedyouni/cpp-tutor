---
title: "Containers and iterators"
slides: "153–157"
---

The same loop, written five ways (slides 153–157). The progression shows the abstraction the STL is built on.

```cpp
// 1. C, by index
int tab[10];
for( int i = 0; i < 10; ++i ) tab[i] = 1;
// 2. C, by pointer: start, end (one PAST the last), increment, dereference
for( int * p = tab; p != tab + 10; ++p ) *p = 1;
// 3. C++: a vector, and an ITERATOR with exactly the pointer's interface
std::vector< int > tab( 10 );
for( std::vector< int >::iterator p{ tab.begin() }; p != tab.end(); ++p ) *p = 1;
// 4. same, with auto and the free functions std::begin / std::end (which also work on raw arrays)
for( auto p{ std::begin( tab )}; p != std::end( tab ); ++p ) *p = 1;
// 5. range-for: the compiler writes version 4 for you
for( auto & p : tab ) p = 1;
```

An **iterator** generalises a pointer: `*it` reads the element, `++it` moves on, `==`/`!=` compare positions. Every container provides `begin()` (the first element) and `end()` (one *past* the last), forming a half-open range `[begin, end)`. A `std::list` iterator follows links and a `std::map` iterator walks a tree, but the loop is written the same way.

::example{id="iteration-styles"}

:::warning
Modifying a container can **invalidate** iterators, pointers and references into it. `push_back` on a `vector` that needs to grow moves every element to a new buffer. This bug is common and serious, and ASan catches it.
:::

::example{id="iterator-invalidation"}
