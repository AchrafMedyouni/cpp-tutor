`class SmaCrossStrategy(std::size_t fast, std::size_t slow)` with `void on_tick(const Tick&)` and `const std::vector<Signal>& signals() const`:

- for **each symbol separately**, once `slow` prices have been seen, compare the simple moving averages of the last `fast` and the last `slow` prices
- emit `Signal{ symbol, buy = true, price }` when the fast SMA **crosses above** the slow one, and `buy = false` when it crosses below. The first comparison only records the state, and equal SMAs don't change it.

Plug it into the pipeline: `run_pipeline(lines, [&](const Tick& t) { strategy.on_tick(t); })`.
