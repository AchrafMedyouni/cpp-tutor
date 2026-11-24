#include <iostream>

int main() {
  double closes[5] = { 100, 101, 102, 103, 104 };
  double sum = 0;
  for (int i = 0; i <= 5; ++i)   // off-by-one: i == 5 is past the end
    sum += closes[i];
  std::cout << sum / 5 << "\n";
}
