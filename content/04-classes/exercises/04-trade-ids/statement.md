Write `class Trade` (slide 119):

- `Trade(std::string symbol, long qty)`: each new trade gets a unique, increasing `id()` (1, 2, 3, …)
- a **copy** of a trade is a new trade: same symbol and quantity, **new id**
- `static long live()`: how many `Trade` objects currently exist
- `static void reset_ids()`: the next id becomes 1 again (for tests)
