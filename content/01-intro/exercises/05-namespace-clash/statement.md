Two market-data vendors ship headers that both define `bid(double)` and `name()` at global scope. Including both breaks the build.

1. Put each vendor's functions in its own namespace: `vendor_a` and `vendor_b`.
2. In `desk.hpp`, inside a namespace `desk`, write `inline double best_bid(double spot)`, which returns the **higher** of the two vendors' bids.
3. Still in `desk.hpp`, at global scope, define a namespace alias `va` for `vendor_a`.

The tests call `vendor_a::bid`, `vendor_b::name`, `desk::best_bid` and `va::name`.
