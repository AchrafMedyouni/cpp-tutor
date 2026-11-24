The hierarchy in `instruments.hpp` is given (read-only). Write, with **no raw `new`/`delete`**:

- `std::unique_ptr<Instrument> make_instrument(const std::string& spec)`: `"BOND 100 0.05"` gives a `Bond`, `"EQUITY 189.5"` an `Equity`, `"TRACKED 10"` a `Tracked`. Anything else, or bad numbers, gives `nullptr`.
- `double book_value(const std::vector<std::unique_ptr<Instrument>>& book)`: the sum of prices
- `std::unique_ptr<Instrument> take_largest(std::vector<std::unique_ptr<Instrument>>& book)`: **transfers ownership** of the most expensive instrument out of the book and removes its slot. Returns `nullptr` if the book is empty.
