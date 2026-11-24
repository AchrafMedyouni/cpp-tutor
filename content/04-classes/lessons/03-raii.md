---
title: "Owning a resource: Tableau v1, v2 and RAII"
slides: "98–103"
---

`Complexe` holds plain values. Most interesting classes **acquire resources** when they are initialised: heap memory, a file, a lock, a socket. Those resources must be released exactly once. To study this, the course builds a `Tableau`: an array of `double` that knows its size and checks indices. (A real project would use `std::vector`.)

## Tableau v1: a leak

```cpp
class Tableau {
public:
  Tableau( unsigned taille ) { taille_ = taille; tab_ = new double[taille]; }
  double & get( unsigned indice ) { /* check */ return tab_[indice]; }
  unsigned taille() const { return taille_; }
private:
  unsigned int taille_;
  double * tab_;
};
```

`get` returns a `double&` (a reference to the element), so `tab.get(i) = d;` writes into the array. But nothing ever calls `delete[]`. **Every `Tableau` leaks its buffer**, and there is no garbage collector to save you.

::example{id="tableau-v1"}

## The destructor

The **destructor** is the complement of the constructor: it *de-initialises* the object and releases the resources acquired by the constructor or by other methods.
- Its name is the class name prefixed by `~`: `~Tableau()`.
- It takes no arguments, so there is exactly one destructor (no overloading).
- **The compiler calls it automatically** whenever an instance is destroyed: at the end of the block for automatic objects, on `delete` for dynamic ones, after `main` for statics.
- The destructor the compiler generates by default calls the destructors of the members. For primitive members, **including pointers**, that does nothing.

```cpp
~Tableau() { delete[] tab_; }   // delete[], not delete: the compiler can't know tab_ points to an array
```

::example{id="tableau-v2"}

::example{id="lifetime-order"}

## RAII: *Resource Acquisition Is Initialisation*

This is the most important idiom in C++: **the constructor acquires the resource and the destructor releases it**. Because destructors run automatically and deterministically, at the end of the scope, even when an exception is thrown, the resource can't leak. Two standard examples from slide 103:

```cpp
{ std::ifstream data{ "input.txt" }; /* ... */ }   // the file is closed at }
{ std::lock_guard lock{ mutex };    /* ... */ }    // the mutex is unlocked at }   (chapter 8)
```

Python reaches for `with` statements and context managers to get the same guarantee. In C++ it's built into every object.

::example{id="raii-file"}
