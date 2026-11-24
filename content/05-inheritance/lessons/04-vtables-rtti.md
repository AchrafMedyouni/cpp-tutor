---
title: "How dynamic binding works; RTTI and casts"
slides: "131–134"
---

## Under the hood: the vtable

Every class with virtual functions gets a **virtual table** (*vtable*): a static array of function pointers, one per virtual function. Each **object** of such a class carries a hidden pointer (the *vptr*) to its class's vtable, usually as its first member. `pf->dessine()` compiles to "follow the vptr, take entry #1, call it". That costs one indirection, and the object grows by one pointer.

```
 c (Cercle)                    vtable of Cercle
┌──────────┐                ┌───────────────────┐
│  vptr  ──┼──────────────▶ │ &Cercle::~Cercle  │
│ centre_  │                │ &Cercle::dessine  │
│ rayon_   │                └───────────────────┘
└──────────┘
 r (Rectangle)                 vtable of Rectangle
┌──────────┐                ┌──────────────────────┐
│  vptr  ──┼──────────────▶ │ &Rectangle::~Rect…   │
│ centre_  │                │ &Rectangle::dessine  │
│ largeur_ │                └──────────────────────┘
│ longueur_│                   vtable of Forme (abstract)
└──────────┘                ┌──────────────────────┐
                            │ &Forme::~Forme       │
                            │ 0  (pure virtual)    │
                            └──────────────────────┘
```

::example{id="vtable-size"}

## RTTI: checked downcasts

**`dynamic_cast`** asks the run-time type information (RTTI), reached through the vptr, so it needs at least one virtual function. It checks whether the object really has the requested type:
- on a **pointer**, it returns `nullptr` if not;
- on a **reference**, it throws `std::bad_cast`.

`static_cast` performs the same conversion **without checking**: it is fast, and UB if you are wrong. `typeid(*pf)` gives the dynamic type.

::example{id="dynamic-cast"}

:::course
Slide 133 catches `std::bad_cast e` **by value**. Catch exceptions by `const&` (`catch (const std::bad_cast& e)`). It avoids a copy, and it avoids slicing when the exception is a derived class.
:::

## The four C++ casts (slide 134)

| Cast | Use | Checked? |
|---|---|---|
| `static_cast<T>(x)` | well-defined conversions: `double`→`int`, base*→derived* when you *know* | at compile time only |
| `dynamic_cast<T>(x)` | polymorphic downcast | at run time |
| `const_cast<T>(x)` | removes `const`/`volatile` (writing to a truly const object stays UB) | no |
| `reinterpret_cast<T>(x)` | reinterpret the bits, e.g. `int*`→`char*` | no |
| `(T) x`, `T(x)` | C cast / functional notation: tries the casts above in turn. Avoid it, because it hides which one happens | no |

Named casts are verbose on purpose: they are easy to search for, and each states its intent.

::example{id="casts"}
