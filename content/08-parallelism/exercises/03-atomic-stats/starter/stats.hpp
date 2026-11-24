#pragma once
#include <atomic>
#include <climits>

class LatencyStats {
public:
  void add(long micros) { ++count_; sum_ += micros; if (micros > max_) max_ = micros; }   // TODO: not thread-safe
  long count() const { return count_; }
  long sum() const { return sum_; }
  double mean() const { return count_ ? double(sum_) / count_ : 0.0; }
  long max() const { return max_; }
private:
  long count_ = 0, sum_ = 0, max_ = LONG_MIN;
};
