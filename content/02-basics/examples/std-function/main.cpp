#include <cmath>
#include <functional>
#include <iostream>

long partie_entiere( double d ) { return static_cast<long>( d ); }   // truncates towards 0
long arrondi( double d ) { return std::lround( d ); }

void bar( std::function< long( double ) > f ) {
  std::cout << "bar: f(7.9) = " << f( 7.9 ) << "\n";
}

int main() {
  std::function< long( double ) > foo;
  foo = partie_entiere;                     // affectation
  std::cout << "foo(2.5) = " << foo( 2.5 ) << "\n";   // appel
  std::cout << "foo(-2.5) = " << foo( -2.5 ) << "\n";
  bar( partie_entiere );
  foo = arrondi;                            // same type, different function
  std::cout << "now foo rounds: " << foo( 2.5 ) << "\n";
}
