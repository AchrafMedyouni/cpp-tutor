Write `template <typename T> class BlockingQueue` for producer–consumer pipelines (slides 204–208):

- `explicit BlockingQueue(std::size_t capacity)`
- `bool push(T value)`: blocks while full; returns `false` (without pushing) if the queue is closed
- `std::optional<T> pop()`: blocks while empty; returns `std::nullopt` when closed **and** drained
- `void close()`: wakes up every waiting thread
- `std::size_t size() const`
