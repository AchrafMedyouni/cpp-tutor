---
title: "Strings and named constants"
slides: "35–36"
---

## `std::string`

`std::string` (header `<string>`) is a class from the standard library with **value semantics**. You can concatenate with `+`, compare with `==` and `<`, and copy with `=`. It owns its characters and frees them automatically.

::example{id="std-string"}

## C strings (historical)

In C, a string is an **array of `char` terminated by a sentinel `'\0'`**: `char s[] = "Hello world!";` occupies 13 bytes. Arrays can't be assigned with `=`, and `==` compares **addresses**, not contents. You use `<cstring>` functions instead: `strlen`, `strcpy`, `strcat`, `strcmp`. You will meet C strings in old APIs and in `main(int argc, char* argv[])`. Wrap them in `std::string` as soon as possible.

::example{id="c-string"}

## Named constants

```cpp
const int taille{ x };        // can't be modified; x may only be known at run time
constexpr int taille{ 10 };   // computed by the compiler; usable where a compile-time constant is required
#define TAILLE 10             // historical C: text substitution by the preprocessor, no type, no scope
```

`constexpr` also applies to **functions**: if the arguments are known at compile time, the result is computed during compilation.

::example{id="constants"}
