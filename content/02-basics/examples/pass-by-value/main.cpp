#include <iostream>

void f1( int i ) {
  int k{ i };
  i = 3;                    // modifies the local copy only
  std::cout << "f1: received " << k << ", modified copy to " << i << "\n";
}

int main() {                // the slide's g1()
  f1( 5 );
  int j{ 8 };
  f1( j );
  f1( j + 2 );
  std::cout << "after calls, j is still " << j << "\n";
}
