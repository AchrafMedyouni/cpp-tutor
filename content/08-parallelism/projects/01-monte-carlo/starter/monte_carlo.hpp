#pragma once
#include <atomic>
#include <cmath>
#include <cstdint>
#include <mutex>
#include <thread>
#include <vector>
#include "mc_core.hpp"

// Step 1
inline double mc_price_seq(const McParams& p, long paths, std::uint64_t seed) { (void)p; (void)paths; (void)seed; return 0; }   // TODO
// Step 2
inline double mc_price_threads(const McParams& p, long paths, unsigned nthreads, std::uint64_t seed) {   // TODO
  (void)p; (void)paths; (void)nthreads; (void)seed;
  return 0;
}
// Step 3
inline double mc_price_mutex(const McParams& p, long paths, unsigned nthreads, std::uint64_t seed, long batch = 1000) {   // TODO
  (void)p; (void)paths; (void)nthreads; (void)seed; (void)batch;
  return 0;
}
inline double mc_price_atomic(const McParams& p, long paths, unsigned nthreads, std::uint64_t seed, long batch = 1000) {   // TODO
  (void)p; (void)paths; (void)nthreads; (void)seed; (void)batch;
  return 0;
}
