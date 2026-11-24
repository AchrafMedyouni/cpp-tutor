Write `long long parallel_sum(const std::vector<int>& v, unsigned nthreads)` using **`std::thread`**. Split the vector into `nthreads` contiguous slices, sum each slice in its own thread, then combine.

Graded under **ThreadSanitizer**: any data race fails, even when the total happens to be right.
