Keep the last `N` prices, as for a moving average. Write `class PriceRing`:

- `explicit PriceRing(std::size_t capacity)`: throws `std::invalid_argument` if `capacity == 0`
- `void push(double p)`: when full, overwrites the **oldest** price
- `std::size_t size() const` (≤ capacity), `std::size_t capacity() const`
- `double at(std::size_t i) const`: `i = 0` is the **oldest** stored price; throws `std::out_of_range` if `i >= size()`
- `double mean() const` (0 when empty)
- it owns a `new[]` buffer, so it needs the **rule of three**
