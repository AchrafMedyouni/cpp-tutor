#include <cmath>
#include <iostream>

int main() {
  std::cout << "7 / 2 = " << 7 / 2 << ", 7 % 2 = " << 7 % 2 << "\n";
  std::cout << "-7 / 2 = " << -7 / 2 << ", -7 % 2 = " << -7 % 2 << "\n";
  std::cout << "7.0 / 2 = " << 7.0 / 2 << "\n";        // one double operand: floating division
  std::cout << "2^10 = " << std::pow(2, 10) << "\n";   // no ** operator in C++
}
