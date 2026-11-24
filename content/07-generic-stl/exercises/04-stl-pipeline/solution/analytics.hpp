#pragma once
#include <algorithm>
#include <cstddef>
#include <functional>
#include <numeric>
#include <vector>

inline std::vector<double> simple_returns(const std::vector<double>& p) {
  if (p.size() < 2) return {};
  std::vector<double> r(p.size() - 1);
  std::transform(p.begin() + 1, p.end(), p.begin(), r.begin(), [](double today, double yday) { return today / yday - 1; });
  return r;
}

inline double mean(const std::vector<double>& x) {
  return x.empty() ? 0.0 : std::accumulate(x.begin(), x.end(), 0.0) / x.size();
}

inline std::vector<double> top_n(const std::vector<double>& x, std::size_t n) {
  std::vector<double> v = x;
  n = std::min(n, v.size());
  std::partial_sort(v.begin(), v.begin() + n, v.end(), std::greater<>{});
  v.resize(n);
  return v;
}

inline std::size_t count_up_days(const std::vector<double>& p) {
  if (p.size() < 2) return 0;
  return std::inner_product(p.begin() + 1, p.end(), p.begin(), std::size_t{ 0 }, std::plus<>{},
                            [](double today, double yday) { return today > yday ? 1u : 0u; });
}

inline double max_drawdown(const std::vector<double>& p) {
  std::vector<double> peak(p.size());
  std::partial_sum(p.begin(), p.end(), peak.begin(), [](double a, double b) { return std::max(a, b); });
  return std::transform_reduce(p.begin(), p.end(), peak.begin(), 0.0,
                               [](double a, double b) { return std::max(a, b); },
                               [](double price, double pk) { return 1 - price / pk; });
}
