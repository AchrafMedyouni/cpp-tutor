#include <iostream>

void legacy_print(int* p) { std::cout << "const_cast: " << *p << "\n"; }   // an old API that forgot const

int main() {
  auto d{ 2.7 };
  auto i = static_cast< int >( d );                 // changes representation
  std::cout << "static_cast<int>(2.7) = " << i << "\n";

  auto const j = 2;
  auto p = const_cast< int * >( &j );               // removes const: fine for READING only
  legacy_print( p );

  int one{ 1 };
  int * pi{ &one };
  auto q = reinterpret_cast< unsigned char * >( pi );   // view the int's bytes
  std::cout << "first byte of 1: " << int(q[0]) << (q[0] == 1 ? " (little-endian)" : " (big-endian)") << "\n";

  int a = ( int ) d;                                // préfixée, historique C
  int b = int( d );                                 // notation fonctionnelle
  std::cout << "C cast (int)2.7 = " << a << ", int(2.7) = " << b << "\n";
}
