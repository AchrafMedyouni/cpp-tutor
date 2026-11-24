Write, in `tvm.hpp`:

- `double future_value(double pv, double rate, int years, int periods_per_year = 1)`: `pv · (1 + rate/m)^(years·m)`
- `double present_value(double fv, double rate, int years, int periods_per_year = 1)`
- `double annuity_payment(double pv, double rate_per_period, int n_periods)`: the constant payment that repays `pv` over `n_periods` (loan formula). If the rate is 0, it's just `pv / n`.

The default argument (slide 48) makes annual compounding the default.
