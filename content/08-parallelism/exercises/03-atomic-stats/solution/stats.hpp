#pragma once
#include <atomic>
#include <climits>

class LatencyStats {
public:
  void add(long micros) {
    count_.fetch_add(1);
    sum_.fetch_add(micros);
    long cur = max_.load();
    while (micros > cur && !max_.compare_exchange_weak(cur, micros)) {
      // cur was reloaded by compare_exchange_weak: retry while we are still larger
    }
  }
  long count() const { return count_.load(); }
  long sum() const { return sum_.load(); }
  double mean() const {
    long n = count_.load();
    return n ? double(sum_.load()) / n : 0.0;
  }
  long max() const { return max_.load(); }
private:
  std::atomic<long> count_{ 0 }, sum_{ 0 }, max_{ LONG_MIN };
};
