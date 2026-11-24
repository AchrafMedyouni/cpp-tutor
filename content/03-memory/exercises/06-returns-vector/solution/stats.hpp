#pragma once
#include <cmath>
#include <vector>

inline std::vector<double> log_returns(const std::vector<double>& prices) {
  std::vector<double> r;
  for (std::size_t i = 1; i < prices.size(); ++i) r.push_back(std::log(prices[i] / prices[i - 1]));
  return r;
}

inline double sample_stdev(const std::vector<double>& x) {
  if (x.size() < 2) return 0;
  double mean = 0;
  for (double v : x) mean += v;
  mean /= x.size();
  double ss = 0;
  for (double v : x) ss += (v - mean) * (v - mean);
  return std::sqrt(ss / (x.size() - 1));
}

inline double max_drawdown(const std::vector<double>& prices) {
  double peak = 0, mdd = 0;
  for (double p : prices) {
    if (p > peak) peak = p;
    if (peak > 0 && 1 - p / peak > mdd) mdd = 1 - p / peak;
  }
  return mdd;
}
