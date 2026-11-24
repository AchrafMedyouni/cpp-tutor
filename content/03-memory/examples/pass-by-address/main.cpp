#include <iostream>

void f2( int * i ) {
  int k{ *i };
  std::cout << "f2 saw " << k << "\n";
  *i = 3;                         // writes into the caller's variable
}

// Finance: update both position and cash of the caller.
void apply_buy(int qty, double price, int* position, double* cash) {
  *position += qty;
  *cash -= qty * price;
}

int main() {
  int j{ 8 };
  std::cout << "j before: " << j << "\n";
  f2( &j );
  std::cout << "j after: " << j << "\n";

  int pos{ 100 };
  double cash{ 0 };
  apply_buy(50, 30.0, &pos, &cash);
  std::cout << "position " << pos << ", cash " << cash << "\n";
}
