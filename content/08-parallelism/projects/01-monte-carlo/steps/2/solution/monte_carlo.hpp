#pragma once
#include <atomic>
#include <cmath>
#include <cstdint>
#include <mutex>
#include <thread>
#include <vector>
#include "mc_core.hpp"

inline double discount(const McParams& p, double payoff_sum_total, long paths) {
  return std::exp(-p.rate * p.maturity) * payoff_sum_total / paths;
}

inline double mc_price_seq(const McParams& p, long paths, std::uint64_t seed) {
  std::mt19937_64 rng{ seed };
  return discount(p, payoff_sum(p, paths, rng), paths);
}

// Each thread: its own RNG (seed + t), its own slice of paths, its own slot in `partial`. No sharing → no race.
inline double mc_price_threads(const McParams& p, long paths, unsigned nthreads, std::uint64_t seed) {
  if (nthreads == 0) nthreads = 1;
  std::vector<double> partial(nthreads, 0.0);
  std::vector<std::thread> pool;
  for (unsigned t = 0; t < nthreads; ++t) {
    long n = paths / nthreads + (t < paths % nthreads ? 1 : 0);
    pool.emplace_back([&p, &partial, n, t, seed] {
      std::mt19937_64 rng{ seed + t };
      partial[t] = payoff_sum(p, n, rng);
    });
  }
  for (auto& th : pool) th.join();
  double total = 0;
  for (double s : partial) total += s;
  return discount(p, total, paths);
}

inline double mc_price_mutex(const McParams& p, long paths, unsigned nthreads, std::uint64_t seed, long batch = 1000) {
  (void)batch;
  return mc_price_threads(p, paths, nthreads, seed);   // step 3
}
inline double mc_price_atomic(const McParams& p, long paths, unsigned nthreads, std::uint64_t seed, long batch = 1000) {
  (void)batch;
  return mc_price_threads(p, paths, nthreads, seed);   // step 3
}
