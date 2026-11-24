---
title: From source code to a running program
slides: "8–18"
---

C (Dennis Ritchie, 1972, Bell Labs, written alongside Unix) is often called a *high-level assembler*: it maps almost one-to-one to what the processor does. C++ (Bjarne Stroustrup) is **a superset of C** that adds abstraction: classes, templates, lambdas and the standard library. It keeps C's closeness to the hardware. Its motto is **"pay only for what you use"**: an abstraction you don't use costs nothing at run time.

C++ is standardised by ISO: C++98, C++03, C++11 (the big modernisation), C++14, C++17, **C++20** (the standard used here), C++23 and C++26. It is a complex language because it is *constrained by its history*: every C++ standard must keep accepting decades of old code.

## The machine underneath (Von Neumann)

- **Memory** is an array of bytes. An **address** is an index into that array.
- A byte has no type. Whether it holds an instruction, an `int`, a `double` or an address depends only on **what the processor does with it**.
- To modify a value, the CPU **loads** it into a register, computes, and **stores** it back (*load and store*). This matters again in chapter 8, where it is the root cause of data races.

## Calling a function uses the stack

Each call pushes a **frame** on the *call stack*: the arguments, room for the return value, the **return address** and the function's local variables. Returning pops the frame. Registers hold the values being computed. An **ABI** (*Application Binary Interface*) is the convention every compiled program follows for how frames and registers are laid out, so that code from different compilers can call each other.

::example{id="call-stack"}

## Interpretation, compilation, virtual machine

| | How it runs | Examples |
|---|---|---|
| **Interpretation** | An executable (the *interpreter*) reads your source and asks the CPU/OS to do the equivalent, line after line. | CPython |
| **Compilation** | A *compiler* translates the source once into native **machine code**. The binary then runs directly on the CPU. | C, C++, Rust |
| **Virtual machine** | Source is compiled to **bytecode**, which an interpreter (the VM) executes. | Java, C# |

The slide asks: *how is the Python interpreter written?* In C. CPython is a C program. This is the main reason C/C++ performance matters even to Python users: NumPy, pandas and PyTorch are C/C++ libraries underneath.

Variants of compilation (slide 17):
- **Separate compilation:** each source file is compiled independently into an *object file*, and the object files are then **linked** into an executable. This is the C/C++ model, covered in detail in the next lessons.
- **JIT (just-in-time):** the VM compiles hot bytecode to machine code while the program runs (JVM, V8, PyPy).
- **Cross-compilation:** the target CPU differs from the one running the compiler (e.g. building ARM firmware on an x86 laptop).

::example{id="factorial"}

:::note
Every *Run* in CppLab invokes the real compiler on your machine:
`g++ -std=c++20 -Wall -Wextra -pedantic -g -pthread`. It then links and executes the binary in a temporary directory. The **ASan + UBSan** option adds `-fsanitize=address,undefined`, which instruments the program to catch memory errors and undefined behaviour at run time. You will rely on it a lot from chapter 3 on.
:::
