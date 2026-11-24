- `operator<<` for `Vector` prints `[1, 2.5, 3]`. For `Matrix`, it prints one bracketed row per line (each followed by `\n`).
- `double portfolio_variance(const Matrix& cov, const Vector& w)`: `wᵀ Σ w`. A non-square matrix throws `std::invalid_argument`.

With two assets of volatility 20% and 30%, correlation 0.5 and weights 60/40, the volatility is about 20.78% (variance 0.0432).
