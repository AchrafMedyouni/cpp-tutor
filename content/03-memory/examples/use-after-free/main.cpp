#include <iostream>

int main() {
  double *last_price = new double{ 101.5 };
  delete last_price;
  std::cout << *last_price << "\n";   // the memory no longer belongs to us
}
