// Built with -DDEBUG (see the badge above): the DEBUG blocks are compiled in.
#include <iostream>

double mid_price(double bid, double ask) {
#ifdef DEBUG
  std::cerr << "[debug] mid_price(" << bid << ", " << ask << ")\n";
#endif
  return (bid + ask) / 2;
}

int main() {
  std::cout << mid_price(99.0, 101.0) << "\n";
#ifdef DEBUG
  std::cout << "debug build\n";
#else
  std::cout << "release build\n";
#endif
}
