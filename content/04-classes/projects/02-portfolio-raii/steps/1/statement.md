Write `class Portfolio` (same behaviour as chapter 3's functions):

- `explicit Portfolio(int capacity = 4)`; the **destructor** releases the array
- `int size() const`, `const Position* find(const std::string&) const`
- `void add_fill(const std::string& symbol, long qty, double price)`: grows as needed, with the same average-cost rule
- `double market_value(const std::map<std::string, double>& marks) const`

The data members are **private**: the user can't touch the array anymore, and can't leak it.
