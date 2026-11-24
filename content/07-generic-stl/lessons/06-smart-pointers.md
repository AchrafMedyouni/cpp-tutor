---
title: "Smart pointers"
slides: "174–179"
---

`a->b` is valid if `a` is a pointer to an object with a member `b`, **or if `a`'s class overloads `operator->`**. That overload must be a member taking no argument, and it returns something on which `->` applies again (usually a raw pointer). Add `operator*` and you have an object that **behaves like a pointer** but can run code at the right moments, starting with a destructor that `delete`s. That is a **smart pointer**: RAII applied to dynamic allocation.

::example{id="naive-ptr"}

:::course
Slide 175's `ptr<T>` has a destructor but no copy operations (no rule of three!). Copying a `ptr` therefore deletes the same object twice. The standard smart pointers solve this in two different ways.
:::

::example{id="naive-ptr-copy"}

## `std::unique_ptr<T>`: sole ownership

A `unique_ptr` (header `<memory>`) **owns** its object alone. It is **non-copyable** (its copy operations are `= delete`d) but **movable**: ownership is *transferred* with `std::move`. It's as cheap as a raw pointer. Create one with `std::make_unique<T>(args...)`. **It is the default choice** for owning a heap object, e.g. a polymorphic `Forme` in a container.

::example{id="unique-ptr"}

::visualize{id="unique-ptr"}

## `std::shared_ptr<T>`: shared ownership

A `shared_ptr` keeps a **reference count** in a separate control block. Copying increments it and destroying decrements it, and the object is deleted when the count reaches 0. That's what Python does for every object. Create one with `std::make_shared<T>(args...)` (one allocation for the object and the count together). A `shared_ptr<Cercle>` converts to a `shared_ptr<Forme>`.

::example{id="shared-ptr"}

::visualize{id="shared-ptr"}

## `std::weak_ptr<T>`: observe without owning

Reference counting can't reclaim **cycles**. If A holds a `shared_ptr` to B and B holds one to A, neither count ever drops to 0, and both leak (Python has a cycle collector on top of reference counting; C++ doesn't). A `weak_ptr` refers to an object **without owning it**. `lock()` returns a `shared_ptr`, which is empty if the object is already gone. Use it for back-references (child → parent) and caches.

::example{id="shared-cycle"}

:::modern
Guidelines: use **no raw `new`/`delete`** in application code. `unique_ptr` by default; `shared_ptr` only when ownership is genuinely shared; raw pointers or references for *non-owning* access.
:::
