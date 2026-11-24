#include <iostream>
#include <iterator>
#include <vector>

template< typename Container >               // 1. generic container
void mettre_a_1( Container & c ) {
  for( auto & p : c ) p = 1;
}

template< typename Iter, typename T >         // 3. range + value
void mettre_a_val( Iter beg, Iter end, T val ) {
  for( Iter p{ beg }; p != end; ++p ) *p = val;
}

template< typename Iter, typename F >         // 4. range + operation
void apply( Iter beg, Iter end, const F & fun ) {
  for( Iter p{ beg }; p != end; ++p ) fun( *p );
}

template< typename T >                        // 5. a function object
struct mettre_a_val_f {
  mettre_a_val_f( T val ) : val_{ val } {}
  void operator()( T & t ) const { t = val_; }
  const T val_;
};

void show( const std::vector<int> & v ) { for( int x : v ) std::cout << x << " "; std::cout << "\n"; }

int main() {
  std::vector< int > tab( 4 );
  mettre_a_1( tab );                                     show( tab );
  mettre_a_val( std::begin( tab ), std::end( tab ), 7 ); show( tab );
  apply( std::begin( tab ), std::end( tab ), []( int & v ) { v += 1; } );  show( tab );
  apply( std::begin( tab ), std::end( tab ), mettre_a_val_f<int>( 5 ) );  show( tab );

  int raw[3];
  mettre_a_val( std::begin( raw ), std::end( raw ), 9 ); // works on raw arrays too
  std::cout << "raw array: " << raw[0] << " " << raw[1] << " " << raw[2] << "\n";
}
