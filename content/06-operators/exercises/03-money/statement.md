Money must never be a `double` (lesson *Primitive types*). Write `Money`:

- `Money(long cents, std::string currency)`, `cents()`, `currency()`
- `+`, `-` between amounts of the **same** currency (else throw `std::invalid_argument`); `* long` and `long *`
- `==`, and `<` (throws on a currency mismatch)
- `operator<<` prints `12.34 EUR` and `-0.05 USD`
