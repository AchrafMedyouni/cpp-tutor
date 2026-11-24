#include <iostream>

int main() {
  int total;                       // no initialiser: indeterminate value
  for (int i = 1; i <= 3; ++i)
    total += i;                    // reads garbage the first time
  std::cout << total << "\n";
}
