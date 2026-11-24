#include <iostream>

#define SQUARE_BAD(x) x * x          // macro: pure text substitution
constexpr int square(int x) { return x * x; }

constexpr double compound(double rate, int years) {
  double f = 1.0;
  for (int i = 0; i < years; ++i) f *= 1.0 + rate;
  return f;
}

int main() {
  constexpr int days{ 7 };             // known at compile time
  constexpr int fee_per_day{ 3 };
  std::cout << "weekly fee: " << days * fee_per_day << "\n";

  int years{};
  std::cin >> years;
  const double growth{ compound(0.05, years) };   // const: run-time value, read-only afterwards
  std::cout << "years: " << years << ", compound factor: " << growth << "\n";

  static_assert(square(4) == 16);      // evaluated by the compiler
  std::cout << "bad macro: " << SQUARE_BAD(1 + 2) << " vs good: " << square(1 + 2) << "\n";
}
