`Price` and `Quantity` wrap a `double` and a `long`. As written, a raw number converts silently into either, so `notional(qty, px)` with the arguments swapped compiles (slide 118).

- Make both single-argument constructors **`explicit`**.
- Implement `double notional(Price p, Quantity q)`.
- Add a `Price` validity rule: the constructor throws `std::invalid_argument` if the value is negative.
