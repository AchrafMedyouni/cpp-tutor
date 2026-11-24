`class Matrix`:

- `Matrix(rows, cols)` (zeros) and `Matrix{ { 1, 2 }, { 3, 4 } }` (nested initialiser lists; ragged rows throw `std::invalid_argument`)
- `rows()`, `cols()`, `operator()(i, j)` (const and non-const, bounds-checked), `==`, `transpose()`
- `Matrix * Vector` and `Matrix * Matrix`, which throw `std::invalid_argument` on a dimension mismatch
