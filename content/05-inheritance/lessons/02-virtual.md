---
title: "Dynamic binding, virtual destructors, abstract classes"
slides: "126–128"
---

## The problem: static binding

Suppose `Forme` gets a `void dessine();` so that we can write `pf->dessine()` for any shape. With an ordinary method, the call is resolved **at compile time from the static type** of `pf`, which is `Forme*`. So `Forme::dessine` runs, even when `pf` points to a `Cercle`.

::example{id="static-binding"}

## `virtual`: dynamic binding

Declare the method **`virtual`** in the base class. The call is then resolved **at run time from the dynamic type** of the object (*dynamic binding*, or polymorphism). In Python every method behaves like this. In C++ you choose, because a virtual call has a small cost (*pay only for what you use*).

```cpp
class Forme {
public:
  virtual ~Forme();          // ← virtual destructor!
  virtual void dessine();
};
```

Mark overriding methods with **`override`** (C++11, not on the slides). The compiler then checks that the method really overrides a virtual one, and catches typos and signature mismatches.

::example{id="dynamic-binding"}

## Virtual destructor

If a class is meant to be used polymorphically, its **destructor must be virtual**. Otherwise `delete pf;` (where `pf` is a `Forme*` pointing to a `Cercle`) calls only `~Forme`: the `Cercle` part is never destroyed, and that is formally undefined behaviour. In practice, resources owned by the derived class leak.

::example{id="missing-virtual-dtor"}

## Abstract classes

A `Forme` on its own has no shape to draw. Make `dessine` **pure virtual** with `= 0`. The class becomes **abstract**: it can't be instantiated, and every concrete derived class must implement `dessine`. This is the C++ equivalent of Python's `abc.ABC` + `@abstractmethod`, or a Java interface.

```cpp
virtual void dessine() = 0;
```

::example{id="abstract-class"}

::example{id="payoff-hierarchy"}
