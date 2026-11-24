`mc_core.hpp` (read-only) gives you `bs_call(p)`, the closed-form price, and `payoff_sum(p, paths, rng)`, which simulates `paths` terminal prices under geometric Brownian motion and sums the call payoffs.

Write `double mc_price_seq(const McParams& p, long paths, std::uint64_t seed)`: the discounted average payoff, `e^{−rT}·Σ/paths`, using a `std::mt19937_64` seeded with `seed`. With 100,000 paths the standard error is about 0.05, and the tests allow 0.25 (5 standard errors). The same seed must give the same price.
