- `Future(name, maturity)`: fair price `S·e^{rT}`
- `class Book`: **owns** a list of `Instrument*` with quantities
  - `void add(Instrument* instrument, double quantity)`: takes ownership; the book `delete`s it
  - `std::size_t size() const`, `double value(const Market&) const`: `Σ qty·price`
  - `double delta(const Market& m, double h = 0.01) const`: the sensitivity of the book's value to the spot, by central finite difference
  - non-copyable (`= delete`), because two books deleting the same instruments would be a double delete

`value` is one loop over `Instrument*`: dynamic binding picks the right `price()`.
