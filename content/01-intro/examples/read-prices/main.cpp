// Read prices until end of input; print count, mean and simple return.
#include <iomanip>
#include <iostream>

int main() {
  double price;
  int count = 0;
  double sum = 0.0, first = 0.0, last = 0.0;

  while (std::cin >> price) {        // false at end of input or on bad data
    if (count == 0) first = price;
    last = price;
    sum += price;
    ++count;
  }

  if (count == 0) {
    std::cerr << "no prices\n";
    return 1;
  }
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "n = " << count << "\n";
  std::cout << "mean = " << sum / count << "\n";
  std::cout << "return = " << 100.0 * (last / first - 1.0) << " %\n";
}
