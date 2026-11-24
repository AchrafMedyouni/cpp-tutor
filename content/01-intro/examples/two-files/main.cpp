#include <iostream>
#include "pricing.hpp"   // declarations only

int main() {
  double p = 100.0;
  std::cout << "with fees: " << with_fees(p) << "\n";
  std::cout << "fee rate:  " << fee_rate() << "\n";
}
