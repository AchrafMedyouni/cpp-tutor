Write `class LatencyStats`, updated concurrently by many threads **without a mutex** (slide 203):

- `void add(long micros)`
- `long count() const`, `long sum() const`, `double mean() const` (0 when empty)
- `long max() const`: the largest value seen. This one needs `compare_exchange_weak`.
