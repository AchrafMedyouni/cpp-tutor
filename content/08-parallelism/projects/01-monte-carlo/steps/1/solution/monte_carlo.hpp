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

inline double mc_price_threads(const McParams& p, long paths, unsigned nthreads, std::uint64_t seed) {
  (void)nthreads;
  return mc_price_seq(p, paths, seed);   // not parallel yet (step 2)
}

inline double mc_price_mutex(const McParams& p, long paths, unsigned nthreads, std::uint64_t seed, long batch = 1000) {
  (void)batch;
  return mc_price_threads(p, paths, nthreads, seed);   // step 3
}
inline double mc_price_atomic(const McParams& p, long paths, unsigned nthreads, std::uint64_t seed, long batch = 1000) {
  (void)batch;
  return mc_price_threads(p, paths, nthreads, seed);   // step 3
}
