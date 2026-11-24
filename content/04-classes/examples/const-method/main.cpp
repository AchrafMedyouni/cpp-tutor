#include <iostream>

struct Position {
  int quantity;
  double price;
  double notional() { return quantity * price; }   // forgot const
};

void print(const Position& p) {
  std::cout << p.notional() << "\n";                // error: notional() is not const
}

int main() {
  print(Position{ 10, 25.0 });
}
