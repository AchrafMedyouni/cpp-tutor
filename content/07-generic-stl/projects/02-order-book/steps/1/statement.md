Prices are integer **ticks**. Write `class OrderBook` (non-copyable):

- `std::vector<Trade> add_limit(long id, Side side, long price, long qty)`: in this step, every order simply **rests** in the book (return an empty vector)
- `std::optional<long> best_bid() const` (the highest buy price) and `best_ask()` (the lowest sell price); `std::nullopt` when that side is empty
- `long volume_at(Side side, long price) const`: the total resting quantity at that level
- `std::size_t order_count() const`

Store the orders so that each price level keeps its orders in **arrival order**: you'll need that for time priority. The book must own its orders (no leaks: the tests count allocations).
