#pragma once
#include <algorithm>
#include <vector>

class Payoff {
public:
  virtual ~Payoff() = default;
  // TODO: pure virtual operator()
};

// TODO: Call, Put, Digital, Straddle

inline double expected_payoff(const Payoff& p, const std::vector<double>& scenarios) {
  (void)p; (void)scenarios;
  return 0;   // TODO
}
