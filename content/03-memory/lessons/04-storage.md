---
title: "Where data lives: static, automatic, dynamic"
slides: "65–68"
---

A process's memory is split into regions (slide 65):

```
 ┌──────────────────┐ high addresses
 │      Stack  ↓    │  frames of active calls: parameters, locals
 │                  │
 │      Heap   ↑    │  dynamically allocated objects
 ├──────────────────┤
 │  Global data     │  globals and statics
 ├──────────────────┤
 │      Code        │  machine instructions
 └──────────────────┘ low addresses
```

| Allocation | What | Created | Destroyed |
|---|---|---|---|
| **static** | globals, and locals marked `static` | before `main`'s first instruction (or at first use); zero-initialised by default | after `main` returns |
| **automatic** | ordinary locals and parameters | when the definition is reached; **no default initialisation** | when the enclosing block exits |
| **dynamic** | objects created with `new` | when the programmer asks; **no default initialisation** | when the programmer calls `delete` |

::example{id="storage-durations"}

## `new` and `delete`

```cpp
int *p{ new int{} };   // allocate on the heap; {} initialises to 0
*p = 5;
delete p;              // releases the memory (it isn't "destroyed": p still holds the old address!)
```

C++ has **no garbage collector**, unlike Java, C# and Python. Every `new` needs exactly one matching `delete`, and three things go wrong:

| Bug | What happens | Detected by |
|---|---|---|
| **leak**: `new` without `delete` | memory is never given back | the allocation tracker (*Track allocations*) |
| **use after free**: using `*p` after `delete p` | reads or writes memory that may already be reused | ASan |
| **double delete** | the allocator's bookkeeping is corrupted | ASan |

::example{id="new-delete"}

::visualize{id="new-delete-leak"}


::example{id="leak"}

::example{id="use-after-free"}

::example{id="double-delete"}

::visualize{id="dangling"}


:::modern
Chapter 4 (RAII: destructors) and chapter 7 (`std::unique_ptr`, `std::shared_ptr`) show how modern C++ makes `delete` almost disappear from everyday code. You still need to understand it, because those tools are built on it.
:::
