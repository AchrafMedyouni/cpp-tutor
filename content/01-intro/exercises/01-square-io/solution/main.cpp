#include <iostream>

int main() {
  int n;
  if (std::cin >> n) {
    std::cout << "square of " << n << " = " << n * n << "\n";
  } else {
    std::cout << "invalid input\n";
  }
  return 0;
}
