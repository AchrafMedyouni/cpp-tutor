#include <functional>
#include <iostream>

std::function<double(double)> make_fee_adder(double rate) {
  double fee = rate * 100;           // local variable of this call
  return [&fee](double px) { return px + fee; };   // captures a reference to it...
}                                     // ...and fee dies here

int main() {
  auto add_fee = make_fee_adder(0.01);
  std::cout << add_fee(50.0) << "\n";   // reads a dead stack variable
}
