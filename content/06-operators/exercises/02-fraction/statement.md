Write an exact rational number `Fraction(long num, long den = 1)`:

- always **normalised**: reduced, denominator > 0 (`Fraction{2, -4}` is `-1/2`); throws `std::invalid_argument` if `den == 0`
- `num()`, `den()`; arithmetic `+ - * /` (free functions); division by zero throws
- `==` and `<=>` (C++20): `1/3 < 1/2`
- `std::ostream& operator<<`: prints `3/4`, or just `2` when the denominator is 1
