#pragma once
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <execution>
#include <functional>
#include <numeric>
#include <vector>

inline double sum_of_squares(const std::vector<double>& v) {
  return std::transform_reduce(std::execution::par, v.begin(), v.end(), 0.0, std::plus<>{},
                               [](double x) { return x * x; });
}

inline void normalize(std::vector<double>& v) {
  if (v.empty()) return;
  double mean = std::reduce(std::execution::par, v.begin(), v.end(), 0.0) / v.size();
  std::transform(std::execution::par, v.begin(), v.end(), v.begin(), [mean](double x) { return x - mean; });
}

inline std::size_t count_outliers(const std::vector<double>& v, double threshold) {
  return static_cast<std::size_t>(
      std::count_if(std::execution::par, v.begin(), v.end(), [threshold](double x) { return std::fabs(x) > threshold; }));
}
