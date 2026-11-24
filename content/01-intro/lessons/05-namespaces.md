---
title: Namespaces
slides: "23–24"
---

Two libraries that both define a function called `price` would clash, because a given signature can only be defined once. Namespaces prefix names with a scope, much like Python modules:

```cpp
namespace fr::centralesupelec {        // nested namespaces in one line since C++17
  class Etudiant { /* ... */ };
}
```

- The `std` namespace is reserved for the standard library: `std::cout`, `std::string`, `std::vector`.
- Namespaces are **independent from files**. The same namespace can be reopened in many headers, and one file can contain several namespaces. Python ties module to file; C++ does not.

There are four ways to refer to a name (slide 24):

| Form | Python analogue |
|---|---|
| `fr::centralesupelec::Etudiant e;` (qualified) | `import fr.centralesupelec` then `fr.centralesupelec.Etudiant()` |
| `using namespace fr::centralesupelec;` then `Etudiant e;` | `from fr.centralesupelec import *` |
| `using fr::centralesupelec::Etudiant;` then `Etudiant e;` | `from fr.centralesupelec import Etudiant` |
| `namespace cs = fr::centralesupelec;` then `cs::Etudiant e;` | `import fr.centralesupelec as cs` |

::example{id="namespaces"}

:::warning
Never put `using namespace std;` (or any `using namespace`) in a **header**. Every file that includes it inherits the pollution, and name clashes appear far from their cause. In a `.cpp`, or inside a function, it is a matter of taste.
:::
