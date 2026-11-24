#include <iomanip>
#include <iostream>
#include "config.hpp"

double discount_factor(int years) {
  double df{ 1.0 };
  for (int y = 0; y < years; ++y)
    df /= 1.0 + risk_free_rate;
  return df;
}

int main() {
  std::cout << std::setprecision(4) << "discount factor over 2y: " << discount_factor(2) << "\n";
}
