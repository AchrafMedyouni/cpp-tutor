#include <cmath>
#include <iomanip>
#include <iostream>

int main() {
  double a = 0.1 + 0.2;
  std::cout << "0.1 + 0.2 == 0.3 ? " << (a == 0.3) << "\n";
  std::cout << std::setprecision(17) << "0.1 + 0.2 = " << a << "\n";

  double total = 0.0;
  for (int i = 0; i < 10; ++i) total += 0.10;   // ten payments of 10 cents
  std::cout << "sum of 10 x 0.10 EUR = " << total << "\n";

  std::cout << "with a tolerance: " << (std::fabs(total - 1.0) < 1e-9 ? "equal" : "different") << "\n";
}
