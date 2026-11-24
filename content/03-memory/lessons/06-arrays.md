---
title: "Arrays, pointer arithmetic, std::vector"
slides: "76–81"
---

## Raw arrays (from C)

```cpp
int tab[taille];                     // static or automatic; taille must be a compile-time constant
auto tab( new int[taille] );         // dynamic: the type of tab is int* !  Release with delete[] tab;
tab[i] = -5;
```

A raw array **doesn't know its size** and **doesn't check the index**: `tab[10]` on a 10-element array silently reads or writes past the end. That is undefined behaviour and a classic security hole. ASan catches it.

::example{id="raw-array"}

::example{id="array-overflow"}

## Pointers and arrays

Pointers into an array support **arithmetic**: `p + i` moves `i` *elements* (not bytes), `p2 - p1` is the distance in elements (type `ptrdiff_t`), and `++p` advances by `sizeof(*p)` bytes.

In almost every expression, the **name of an array decays to a pointer to its first element**: `tab` means `&tab[0]`. Exceptions include `sizeof(tab)` and `&tab`. `tab[i]` is *defined* as `*(tab + i)`. This explains three surprises:
- `new T[n]` returns a pointer to the first element;
- `=` doesn't copy an array, and `==` compares addresses;
- a function receiving an "array" really receives a **pointer**, so it has lost the size.

::example{id="pointer-arithmetic"}

::example{id="array-traversal"}

## The modern arrays: `std::array` and `std::vector`

The standard library has a fixed-size `std::array<T, N>` and a growable `std::vector<T>`. They are **as fast as raw arrays** (same contiguous memory), but they know their `size()`, offer a checked `at(i)`, support `=` and `==`, work with range-`for`, and free their memory automatically. **Use them by default.**

```cpp
std::vector< int > tab( 10 );   // parentheses! 10 zeros (tab{ 10 } would be ONE element, 10)
tab[5] = -2;                    // unchecked
tab.at( 5 ) = -2;               // checked: throws std::out_of_range
auto s{ tab.size() };
tab.resize( 20 );
for( auto & el : tab ) el = 0;  // foreach
```

::example{id="vector-basics"}

## Naming types: `using`

`using Integer = short;` and `using Tab = std::vector< Integer >;` give names to existing types. The C syntax is `typedef short Integer;`. Aliases make code read in domain terms:

::example{id="returns-vector"}
