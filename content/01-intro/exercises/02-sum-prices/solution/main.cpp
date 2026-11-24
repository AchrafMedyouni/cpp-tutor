#include <iomanip>
#include <iostream>

int main() {
  double p;
  int count = 0;
  double total = 0.0, lo = 0.0, hi = 0.0;
  while (std::cin >> p) {
    if (count == 0 || p < lo) lo = p;
    if (count == 0 || p > hi) hi = p;
    total += p;
    ++count;
  }
  if (count == 0) {
    std::cout << "no data\n";
    return 0;
  }
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "count: " << count << "\n"
            << "total: " << total << "\n"
            << "mean: " << total / count << "\n"
            << "min: " << lo << "\n"
            << "max: " << hi << "\n";
}
