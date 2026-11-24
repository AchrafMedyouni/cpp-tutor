`returns.cpp` contains the definitions of three functions, but `returns.hpp` is empty, so the test file (a **separate translation unit**) can't call them.

Complete the module:

- **`returns.hpp`**: an include guard, the constant `constexpr int trading_days = 252;`, and the **declarations** of
  - `double simple_return(double p0, double p1)`: `p1 / p0 - 1`
  - `double log_return(double p0, double p1)`: `ln(p1 / p0)`
  - `double annualized_vol(double daily_vol)`: `daily_vol * sqrt(trading_days)`
- **`returns.cpp`**: includes `returns.hpp` and keeps the **definitions** (fix any bug you find).

The tests are compiled into their own object file and linked with `returns.o`, exactly as in the lesson.
