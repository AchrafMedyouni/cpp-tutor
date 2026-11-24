`double mc_price_threads(const McParams& p, long paths, unsigned nthreads, std::uint64_t seed)` splits the paths over `nthreads` `std::thread`s:

- each thread has its **own** generator, seeded `seed + t` (sharing one RNG would be a data race, and slow too)
- each thread writes its partial sum into its **own** slot of a `std::vector<double>`; the main thread adds the slots up after `join`
- the result is reproducible for a given `(seed, nthreads)`

Graded under ThreadSanitizer.
