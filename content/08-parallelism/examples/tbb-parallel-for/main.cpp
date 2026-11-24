#include <cmath>
#include <cstddef>
#include <iostream>
#include <vector>
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>

using tbb::blocked_range;
using tbb::parallel_for;

inline void Foo( float & x ) { x = std::sqrt( x ) + 1.0f; }

void SerialApplyFoo( float a[], size_t n ) {
  for( size_t i{ 0 }; i != n; ++i )
    Foo( a[i] );
}

struct ApplyFoo {                                  // slide 212: functor over a range
  float * const a_;
  void operator() ( const blocked_range< size_t > & r ) const {
    for( auto i{ r.begin() }; i != r.end(); ++i ) {
      Foo( a_[i] );
    }
  }
  ApplyFoo( float a[] ) : a_{ a } {}
};

void parallelApplyFoo1( float a[], size_t n ) {
  parallel_for( blocked_range< size_t >{ 0, n }, ApplyFoo( a ));
}

void parallelApplyFoo2( float a[], size_t n ) {    // slide 213: lambda
  parallel_for( size_t( 0 ), n, [=]( size_t i ) { Foo( a[i] ); } );
}

int main() {
  const size_t n = 1'000'000;
  std::vector<float> s( n ), p1( n ), p2( n );
  for( size_t i = 0; i < n; ++i ) s[i] = p1[i] = p2[i] = float( i );
  SerialApplyFoo( s.data(), n );
  parallelApplyFoo1( p1.data(), n );
  parallelApplyFoo2( p2.data(), n );
  std::cout << "serial == functor: " << ( s == p1 ) << "\n";
  std::cout << "serial == lambda: " << ( s == p2 ) << "\n";
}
