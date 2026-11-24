**v1** (slide 99): `explicit Tableau(unsigned taille)` allocates `taille` doubles initialised to 0. `double& get(unsigned indice)` returns a reference to an element and throws `std::out_of_range` if the index is too large. Add `unsigned taille() const`.

(This version leaks its buffer. The next step fixes that.)
