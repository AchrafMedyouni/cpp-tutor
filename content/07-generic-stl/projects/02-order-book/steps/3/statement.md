- `bool cancel(long id)`: removes a resting order (`false` if unknown or already filled). Empty price levels disappear.
- `std::vector<Trade> add_market(long id, Side side, long qty)`: trades against the opposite side at any price. Whatever can't be filled is **discarded**: market orders never rest.

The tests run a sequence of adds, fills and cancels and check that the allocation count returns to its starting value.
