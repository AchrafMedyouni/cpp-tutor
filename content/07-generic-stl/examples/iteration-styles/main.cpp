#include <iostream>
#include <iterator>
#include <list>
#include <vector>

template <typename C>
void show(const C& c) {
  for (auto it = c.begin(); it != c.end(); ++it) std::cout << *it << (std::next(it) == c.end() ? "\n" : " ");
}

int main() {
  int tab[3];
  for( int i = 0; i < 3; ++i ) tab[i] = 1;                       // 1. index
  show( std::vector<int>( std::begin( tab ), std::end( tab ) ) );
  for( int * p = tab; p != tab + 3; ++p ) *p = 2;                // 2. pointer
  show( std::vector<int>( std::begin( tab ), std::end( tab ) ) );

  std::vector< int > v( 3 );
  for( std::vector< int >::iterator p{ v.begin() }; p != v.end(); ++p ) *p = 3;   // 3. iterator
  show( v );
  for( auto p{ std::begin( v )}; p != std::end( v ); ++p ) *p = 4;               // 4. auto + std::begin
  show( v );
  for( auto & p : v ) p = 5;                                                      // 5. range-for
  show( v );

  std::list< double > l{ 1.5, 2.5 };   // a linked list: same loop
  std::cout << "list: ";
  show( l );
}
