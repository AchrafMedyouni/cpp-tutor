A small trading library has three headers:

- `money.hpp` defines `struct Money` and `inline long cents(Money)`;
- `order.hpp` includes `money.hpp` and defines `struct Order` and `notional()`;
- `portfolio.hpp` includes `money.hpp` and defines `struct Portfolio` and `value()`.

The hidden test file does what real code does: it includes `order.hpp` and `portfolio.hpp`, some of them twice. Right now it **doesn't compile**. Make every header safe to include any number of times, **without changing what they define**.

:::note
Functions defined in headers are marked `inline`: this lets the same definition appear in several translation units without a linker error. The guards solve a different problem: a repeated definition inside **one** translation unit.
:::
