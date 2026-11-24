Implement three functions:

1. `double bond_price(double face, double coupon_rate, int years, double y)`: an annual-coupon bond priced at yield `y`.
2. `double bisect(std::function<double(double)> f, double lo, double hi, double tol = 1e-10)`: a root of `f` on `[lo, hi]`. Assume `f(lo)` and `f(hi)` have opposite signs.
3. `double yield_to_maturity(double price, double face, double coupon_rate, int years)`: the `y` in [0, 1] such that `bond_price(...) == price`. Pass a **lambda** to `bisect`.
