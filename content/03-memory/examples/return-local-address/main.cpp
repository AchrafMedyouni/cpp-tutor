#include <iostream>

double* best_bid() {
  double best = 99.5;
  return &best;          // best dies when the function returns
}

int main() {
  double* p = best_bid();
  std::cout << *p << "\n";
}
