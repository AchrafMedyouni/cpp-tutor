#include <cstdio>
#include <utility>
#include <vector>
#include "cpplab/alloc_tracker.h"   // CppLab helper: counts heap allocations

struct Counted {                    // wraps a buffer and counts copies and moves
  std::vector<double> data;
  inline static int cc = 0, ca = 0, mc = 0, ma = 0;
  explicit Counted( std::size_t n ) : data( n ) {}
  Counted( const Counted & o ) : data{ o.data } { ++cc; }
  Counted( Counted && o ) noexcept : data{ std::move( o.data ) } { ++mc; }
  Counted & operator=( const Counted & o ) { data = o.data; ++ca; return *this; }
  Counted & operator=( Counted && o ) noexcept { data = std::move( o.data ); ++ma; return *this; }
  static void reset() { cc = ca = mc = ma = 0; }
};

template< typename T > void swap_copy( T & t1, T & t2 ) { T tmp{ t1 }; t1 = t2; t2 = tmp; }
template< typename T > void swap_move( T & t1, T & t2 ) {
  T tmp{ std::move( t1 ) };
  t1 = std::move( t2 );
  t2 = std::move( tmp );
}

int main() {
  Counted a{ 1'000'000 }, b{ 1'000'000 };
  Counted::reset();
  long before = cpplab::total_allocations();
  swap_copy( a, b );
  std::printf( "copy swap: %d copy ctor, %d copy assign, %d move ctor, %d move assign, %ld allocations\n",
               Counted::cc, Counted::ca, Counted::mc, Counted::ma, cpplab::total_allocations() - before );
  Counted::reset();
  before = cpplab::total_allocations();
  swap_move( a, b );
  std::printf( "move swap: %d copy ctor, %d copy assign, %d move ctor, %d move assign, %ld allocations\n",
               Counted::cc, Counted::ca, Counted::mc, Counted::ma, cpplab::total_allocations() - before );
}
