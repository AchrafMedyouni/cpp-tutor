#pragma once
#include <algorithm>
#include <cmath>
#include <random>

struct McParams {
  double spot, strike, rate, vol, maturity;
};

// Closed-form Black–Scholes call, to check the simulations against.
inline double bs_call(const McParams& p) {
  auto N = [](double x) { return 0.5 * std::erfc(-x / std::sqrt(2.0)); };
  double sq = p.vol * std::sqrt(p.maturity);
  double d1 = (std::log(p.spot / p.strike) + (p.rate + 0.5 * p.vol * p.vol) * p.maturity) / sq;
  return p.spot * N(d1) - p.strike * std::exp(-p.rate * p.maturity) * N(d1 - sq);
}

// Sum of undiscounted call payoffs over `paths` terminal prices, S_T = S·exp((r - σ²/2)T + σ√T·Z).
inline double payoff_sum(const McParams& p, long paths, std::mt19937_64& rng) {
  std::normal_distribution<double> z{ 0.0, 1.0 };
  double drift = (p.rate - 0.5 * p.vol * p.vol) * p.maturity, diff = p.vol * std::sqrt(p.maturity);
  double sum = 0;
  for (long i = 0; i < paths; ++i) sum += std::max(p.spot * std::exp(drift + diff * z(rng)) - p.strike, 0.0);
  return sum;
}
