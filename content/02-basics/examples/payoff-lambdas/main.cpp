#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>

using Payoff = std::function<double(double)>;   // spot -> payoff

Payoff call(double K) { return [K](double S) { return std::max(S - K, 0.0); }; }
Payoff put(double K)  { return [K](double S) { return std::max(K - S, 0.0); }; }
Payoff add(Payoff a, Payoff b) { return [a, b](double S) { return a(S) + b(S); }; }

int main() {
  double K = 100;
  Payoff c = call(K), p = put(K), straddle = add(c, p);
  std::cout << std::left << std::setw(7) << "spot" << std::setw(7) << "call" << std::setw(7) << "put" << "straddle\n";
  std::cout << std::fixed << std::setprecision(2);
  for (double S : {80.0, 100.0, 120.0})
    std::cout << std::setw(7) << std::setprecision(0) << S << std::setprecision(2) << std::setw(7) << c(S)
              << std::setw(7) << p(S) << straddle(S) << "\n";
}
