#include <iostream>

int main() {
  int qty{ 3.7 };   // error: narrowing conversion from double to int
  std::cout << qty << "\n";
}
