// The factorial from slides 14–16, compiled to native code.
#include <iostream>

int factorial(int n) {
  if (n == 0)
    return 1;
  return n * factorial(n - 1);
}

int main() {
  for (int n = 0; n <= 12; ++n)
    std::cout << n << "! = " << factorial(n) << "\n";
}
