---
title: "What inheritance means: the Liskov substitution principle"
slides: "129–130"
---

> *If for each object o1 of type S there is an object o2 of type T such that for all programs P defined in terms of T, the behavior of P is unchanged when o1 is substituted for o2, then S is a subtype of T.*  (Barbara Liskov, 1988)

> *Functions that use pointers or references to base classes must be able to use objects of derived classes without knowing it.*  (Robert C. Martin, 1996)

Public inheritance promises **substitutability**, which is more than "has the same members". Slide 130's classic dilemma: should `Carre` derive from `Rectangle`, or the other way round?

- **`Carre` derives from `Rectangle`.** Code written for rectangles assumes *"doubling the width doubles the area"*. A square must keep its sides equal, so `set_largeur` also changes the height, and the area is multiplied by **4**. The promise is broken.
- **`Rectangle` derives from `Carre`** (adding a `longueur`). Code written for squares assumes *"doubling the side multiplies the area by 4"*, which a rectangle doesn't honour either.

Neither direction is substitutable **for mutable objects**. The mathematical "a square is a rectangle" holds for immutable values only. Good options: make the shapes immutable, or give both a common abstract base (`Forme`) without these setters.

::example{id="square-rectangle"}
