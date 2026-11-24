#include <iostream>
#include <utility>

void f5( int && i ) {
  int k{ i };
  i = 3;                         // allowed: it's our temporary
  std::cout << k << "\n";
}

int main() {
  int         g{ 8 };
  // int &    rg{ 8 };
  const int & crg{ 8 };
  int &&      rd{ 8 };
  int i{ 3 }, j{ 7 };
  int         k1{ i + j };
  // int &    k2{ i + j };
  const int & k3{ i + j };
  int &&      k4{ i + j };
  (void)g; (void)k1;
  std::cout << "crg = " << crg << ", rd = " << rd << ", k3 = " << k3 << ", k4 = " << k4 << "\n";

  std::cout << "f5 got temporary "; f5( 5 );
  j = 8;
  // f5( j );
  std::cout << "f5 got temporary "; f5( j + 2 );
  std::cout << "f5 got std::move(j) "; f5( std::move( j ) );
}
