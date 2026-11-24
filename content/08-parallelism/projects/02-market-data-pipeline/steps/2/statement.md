`template <typename OnTick> PipelineStats run_pipeline(const std::vector<std::string>& lines, OnTick on_tick, std::size_t capacity = 64)`:

- a **producer** `std::thread` parses every line and pushes the valid ticks into a `BlockingQueue<Tick>` (given, read-only) of the requested capacity, counts the rejected lines, then **closes** the queue
- the **calling thread** is the consumer: it pops ticks until the queue is closed and empty, calls `on_tick(tick)` for each one and counts them
- returns `{ received, rejected }`. Ticks must reach `on_tick` in feed order.

Graded under ThreadSanitizer, with a queue capacity of 1 to force constant blocking.
