#include <algorithm>
#include <functional>
#include <iostream>

int main() {
  auto f1 = []( int n ) { return n % 2 == 0; };   // type deduced: bool
  std::function< bool( int ) > f2{ f1 };          // corrected from the slide
  std::cout << "f1(4) = " << f1( 4 ) << ", f2(7) = " << f2( 7 ) << "\n";

  int tab[] = { 1, 2, 3, 4 };
  std::for_each( tab, tab + 4, []( int & n ){ n++; } );   // passed directly
  for (int x : tab) std::cout << x << " ";
  std::cout << "\n";

  std::cout << "evens: " << std::count_if( tab, tab + 4, f1 ) << "\n";
}
