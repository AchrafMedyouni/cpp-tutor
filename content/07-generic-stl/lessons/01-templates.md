---
title: "Templates"
slides: "147–152"
---

Our `Tableau` stores `double`s. A `Tableau` of `int`, of `Complexe` or of `Tableau` would mean copying the class and changing one word. A **template** makes the type a parameter:

```cpp
template< class T >          // or template< typename T >: identical
class Tableau {
public:
  T & operator[]( unsigned indice );
  // ...
};

template< typename T >       // a member defined outside the class repeats the template header
T & Tableau< T >::operator[]( unsigned indice ) { /* ... */ }
```

`Tableau<int>` and `Tableau<Complexe>` are **instantiations**: the compiler generates a separate class for each type used. Because generation happens wherever the template is used, **the whole template, method bodies included, normally lives in the header**. Templates are a compile-time mechanism; Python's duck typing works at run time.

::example{id="class-template"}

## Non-type parameters

A template parameter can also be a **value** known at compile time. That's how `std::array<T, N>` works:

```cpp
template< typename T, unsigned TAILLE = 100 >   // with a default
class Tableau { T tab_[TAILLE]; };              // no new: the size is part of the type
```

::example{id="nontype-param"}

## Function templates and specialisation

```cpp
template< typename T >
inline T max( T val1, T val2 ) { return val1 > val2 ? val1 : val2; }
```

The compiler **deduces** `T` from the arguments: `max( i, j )` with two `int`s gives `T = int`. Each type used must support the operations in the body (here `>`). Otherwise the error appears when that instantiation is compiled.

A **specialisation** provides a different implementation for one particular type (slide 151, for `Complexe`, which has no `>`):

```cpp
template<>
inline Complexe max( Complexe c1, Complexe c2 ) { /* lexicographic order */ }
```

::example{id="function-template"}

:::warning
Naming your own `max` is asking for trouble. When the arguments are `std::` types, *argument-dependent lookup* also considers `std::max`, and the call becomes ambiguous. The example calls `::max` to select the global one explicitly.
:::

::example{id="template-deduction-error"}

## Templates as a functional language

Templates are evaluated by the compiler, recursively, with specialisations acting as the base case. That makes them a small **compile-time functional language**:

```cpp
template< unsigned int N > class Factorial { public: static constexpr int value{ N * Factorial< N-1 >::value }; };
template<>                 class Factorial< 0 > { public: static constexpr int value{ 1 }; };
```

::example{id="factorial-meta"}
