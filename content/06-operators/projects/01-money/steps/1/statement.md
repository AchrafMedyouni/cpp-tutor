Amounts are **integer cents** plus an ISO currency code (never `double`: see *Primitive types*).

- members: `+=`, `-=`, `*=` (by `long`), unary `-`
- free functions: `+`, `-`, `Money * long`, `long * Money`
- `==` and `<=>` (C++20). Any operation mixing currencies throws `std::invalid_argument`, except `==`, which simply returns `false`.
