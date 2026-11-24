Two variants that accumulate into **one shared total**:

- `mc_price_mutex(p, paths, nthreads, seed, batch = 1000)`: protect `total += s` with a `std::mutex` / `lock_guard`
- `mc_price_atomic(...)`: a `std::atomic<double>` updated with a `compare_exchange_weak` loop

Each thread simulates `batch` paths, then adds their sum (one lock or one atomic update per batch, not per path). Try timing the three versions in your own `main()`: per-thread slots need no synchronisation at all, which is why they usually win.
