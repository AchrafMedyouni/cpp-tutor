#pragma once
#include <algorithm>
#include <functional>

using Payoff = std::function<double(double)>;

inline Payoff make_call(double K) {
  (void)K;
  return [](double) { return 0.0; };   // TODO
}

inline Payoff make_put(double K) {
  (void)K;
  return [](double) { return 0.0; };   // TODO
}

inline Payoff combine(Payoff a, Payoff b, double wa, double wb) {
  (void)b; (void)wa; (void)wb;
  return a;                            // TODO
}

inline Payoff make_bull_spread(double K1, double K2) {
  (void)K2;
  return make_call(K1);                // TODO
}
