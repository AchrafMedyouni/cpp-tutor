---
title: "Base and derived classes"
slides: "121–125"
---

The course's example hierarchy (slide 121). Unlike Python's `object` or Java's `Object`, **C++ imposes no root class**:

```
              Forme
          centre : Point
       deplace( Point p )
           △         △
      Cercle         Rectangle
    rayon : int      largeur, longueur : int
    dessine()        dessine()
    efface()         efface()
```

```cpp
class Forme {                          // base class
public:
  explicit Forme( Point centre );
  void deplace( Point nouveau_centre );
protected:                             // accessible to derived classes, not to the outside
  Point centre_;
};

class Cercle : public Forme {          // a Cercle IS A Forme
public:
  Cercle( Point centre, int rayon );
  void dessine();
private:
  int rayon_;
};
```

- **`protected`** sits between `private` and `public`: members are visible to derived classes but not to the outside world.
- **`public` inheritance** means "is a": everything public in `Forme` stays public in `Cercle`. (`private` and `protected` inheritance exist but are rare.)
- A derived class's constructor must initialise its base part first, by **calling a base constructor in its initialiser list**: `: Forme{ centre }, rayon_{ rayon }`. Construction runs base first, then members, then the derived body. Destruction runs in exactly the reverse order.

::example{id="forme-hierarchy"}

## Manipulating instances: slicing and conversions

A `Cercle` *is a* `Forme`, so (slide 124):

```cpp
f = c;          // ✓ but SLICED: only the Forme part of c is copied into f
// c = f;       // ✗ a Forme is not a Cercle
pf = pc;        // ✓ a Forme* may point to a Cercle (upcast)
*pf = *pc;      // ⚠ assigns only the Forme part, through Forme::operator=
// pc = pf;     // ✗ needs an explicit downcast...
pc = (Cercle *) pf;   // ...which compiles but is UB if *pf isn't really a Cercle
Forme & rf{ c };      // ✓ a reference to the base, bound to a derived object
```

::example{id="slicing"}

::example{id="bad-downcast"}

:::course
Slide 124 writes `pc = (Cercle *) pf;` without comment. If `pf` points to a real `Forme`, as it can in that code, using `pc->rayon_` reads memory that doesn't belong to the object. The checked alternative, `dynamic_cast`, comes later in this chapter.
:::
