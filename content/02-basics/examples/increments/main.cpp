#include <iostream>

int main() {
  int i{ 5 };
  int j{ ++i };   // i == 6 and j == 6
  int k{ i-- };   // i == 5 and k == 6
  std::cout << "i = " << i << ", j = " << j << ", k = " << k << "\n";
}
