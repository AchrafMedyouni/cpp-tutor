#include <iostream>

template< unsigned int N >
class Factorial {
public:
  static constexpr int value{ N * Factorial< N-1 >::value };
};

template<>
class Factorial< 0 > {
public:
  static constexpr int value{ 1 };
};

constexpr int factorial( unsigned n ) { return n == 0 ? 1 : n * factorial( n - 1 ); }   // C++11 onwards: simpler

static_assert( Factorial< 10 >::value == 3628800 );
static_assert( factorial( 10 ) == 3628800 );

int main() {
  std::cout << "Factorial<10>::value = " << Factorial< 10 >::value << "\n";
  std::cout << "factorial(10) = " << factorial( 10 ) << "\n";
}
