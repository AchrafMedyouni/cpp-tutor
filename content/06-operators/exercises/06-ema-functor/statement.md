An exponential moving average is a classic **stateful** function object (call operator `()`). Write `class Ema`:

- `explicit Ema(double alpha)`: throws `std::invalid_argument` unless `0 < alpha <= 1`
- `double operator()(double x)`: feeds a new observation and returns the updated average (the first observation initialises it)
- `double value() const`

It then works directly with `std::transform` (chapter 7) or any loop.
