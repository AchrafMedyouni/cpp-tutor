Complete `formes.hpp` (slides 121–130):

- `Forme`: abstract, with a virtual destructor and pure virtual `double aire() const` and `double perimetre() const`
- `Cercle(r)`, `Rectangle(l, h)`, and `Carre(c)` implement both
- `double total_area(const std::vector<const Forme*>& formes)`: one loop, dynamic binding
