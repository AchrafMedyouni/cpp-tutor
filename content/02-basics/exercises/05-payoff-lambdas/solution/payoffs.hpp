#pragma once
#include <algorithm>
#include <functional>

using Payoff = std::function<double(double)>;

inline Payoff make_call(double K) {
  return [K](double S) { return std::max(S - K, 0.0); };
}

inline Payoff make_put(double K) {
  return [K](double S) { return std::max(K - S, 0.0); };
}

inline Payoff combine(Payoff a, Payoff b, double wa, double wb) {
  return [a, b, wa, wb](double S) { return wa * a(S) + wb * b(S); };
}

inline Payoff make_bull_spread(double K1, double K2) {
  return combine(make_call(K1), make_call(K2), 1.0, -1.0);
}
