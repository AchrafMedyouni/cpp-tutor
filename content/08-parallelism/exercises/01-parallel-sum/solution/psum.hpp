#pragma once
#include <thread>
#include <vector>

inline long long parallel_sum(const std::vector<int>& v, unsigned nthreads) {
  if (nthreads == 0) nthreads = 1;
  std::vector<long long> partial(nthreads, 0);
  std::vector<std::thread> threads;
  const std::size_t n = v.size();
  for (unsigned t = 0; t < nthreads; ++t) {
    threads.emplace_back([&, t] {
      std::size_t lo = n * t / nthreads, hi = n * (t + 1) / nthreads;
      long long s = 0;
      for (std::size_t i = lo; i < hi; ++i) s += v[i];
      partial[t] = s;            // each thread writes only its own slot
    });
  }
  for (auto& th : threads) th.join();
  long long total = 0;
  for (long long p : partial) total += p;
  return total;
}
