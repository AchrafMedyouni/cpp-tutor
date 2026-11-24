Given the hierarchy in `formes.hpp` (read-only), write in `filter.hpp`:

- `int count_cercles(const std::vector<const Forme*>& v)`
- `double sum_rayons(const std::vector<const Forme*>& v)`: the sum of the circles' radii
- `bool is_cercle(const Forme& f)`: implement it with `dynamic_cast` **on a reference** and a `try`/`catch` (slide 133)
