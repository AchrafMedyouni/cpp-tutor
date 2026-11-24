---
title: "Lvalues, rvalues and references"
slides: "69–75"
---

## Lvalues and rvalues

Assignment is the defining operator of imperative programming: it modifies state. An expression that can appear **on the left** of `=` is an **lvalue**: it designates a storage location you can name. One that can only appear on the right is an **rvalue**: a temporary value. With `int i; int *p; Complexe c;` (slide 69):

| expression | lvalue? | why |
|---|---|---|
| `0`, `i + 2`, `&i`, `i++` | ✗ | temporary values (`i++` yields a copy of the old value) |
| `i`, `++i`, `i = 2`, `p`, `*p`, `c`, `c.real` | ✓ | each designates an existing object (`++i` and `i = 2` return `i` itself) |

The standard refines the categories (*glvalue*, *xvalue*, *prvalue*). Chapter 7 needs one of them (xvalues, through `std::move`). Until then, lvalue and rvalue are enough.

## References in C++

"Reference" means a **link to a data item**, but the word means different things in different languages. A Python or Java reference is close to a C++ **pointer**: copying it doesn't copy the object. A **C++ reference** is a **new name for an existing object**, an *alias*. It must be initialised, it can never be re-seated to another object, and it has no address of its own (the slides note it corresponds to nothing in a Von Neumann machine).

::example{id="references"}

## Passing by reference

```cpp
void f3( int & i ) { i = 3; }        // i IS the caller's argument
f3( j );                             // ✓ lvalue
// f3( 5 );  f3( j + 2 );            // ✗ rvalues can't bind to int&
void f4( const int & i ) { ... }     // read-only alias
f4( 5 ); f4( j ); f4( j + 2 );       // ✓ all fine
```

`const T&` combines the **syntax and semantics of passing by value** (the callee can't modify the argument, and you can pass any expression) with the **performance of passing by address** (no copy). It is the default way to pass anything bigger than a few machine words: strings, vectors, your own classes.

::example{id="pass-by-reference"}

::visualize{id="pass-by"}

::example{id="const-ref"}

## Returning a reference

A function returning `T&` returns an **lvalue**, so it can appear on the left of `=` (e.g. `getVal() = 8;`). As with pointers, the object must outlive the call. `std::vector::operator[]` returns a reference, which is why `v[i] = x` works.

::example{id="return-reference"}

## Rvalue references (preview)

`T&&` binds **only to rvalues**, i.e. temporaries about to be destroyed. Its purpose is to let a function *steal* the internals of a temporary instead of copying them (*move semantics*), which is covered in chapter 7.
