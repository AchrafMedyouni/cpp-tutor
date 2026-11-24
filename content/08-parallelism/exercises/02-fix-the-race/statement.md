`FillCounter` counts fills per symbol in a `std::map`. Several threads call `record()` at the same time. A `std::map` isn't thread-safe: concurrent insertions corrupt it, and even the `int` increments race.

Make the class **thread-safe** with a `std::mutex` and `std::lock_guard` (slides 200–202). Both `record()` and `count()` must be protected.
