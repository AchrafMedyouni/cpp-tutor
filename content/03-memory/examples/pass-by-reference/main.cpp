#include <iostream>

void f3( int & i ) {
  int k{ i };
  std::cout << "f3 saw " << k << "\n";
  i = 3;              // modifies the caller's variable
}

void my_swap( int & a, int & b ) {
  int tmp{ a };
  a = b;
  b = tmp;
}

void apply_buy(int qty, double price, int& position, double& cash) {
  position += qty;
  cash -= qty * price;
}

int main() {
  // f3( 5 );
  int j{ 8 };
  f3( j );
  // f3( j + 2 );
  std::cout << "j = " << j << "\n";

  int a{ 1 }, b{ 2 };
  my_swap( a, b );
  std::cout << "a = " << a << ", b = " << b << "\n";

  int pos{ 100 };
  double cash{ 0 };
  apply_buy(50, 30.0, pos, cash);
  std::cout << "position " << pos << ", cash " << cash << "\n";
}
