`class Vector` of `double`s:

- `explicit Vector(std::size_t n = 0)` (zeros) and `Vector{ 1.0, 2.0, 3.0 }` (initialiser list)
- `size()`, `operator[]` (const and non-const; out of range throws `std::out_of_range`)
- `+=`, `-=`, `*=` (scalar); free `+`, `-`, `Vector * double`, `double * Vector`; `==`
- `double dot(const Vector&, const Vector&)`

Size mismatches throw `std::invalid_argument`.
