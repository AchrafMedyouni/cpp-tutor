#include <algorithm>
#include <chrono>
#include <cmath>
#include <execution>
#include <iostream>
#include <numeric>
#include <vector>

template <typename Policy>
double run( const Policy & policy, const std::vector<double> & v, const char * name ) {   // policies aren't copyable
  auto t0 = std::chrono::steady_clock::now();
  double r = std::transform_reduce( policy, v.begin(), v.end(), 0.0, std::plus<>{},
                                    []( double x ) { return std::sqrt( x ) * std::log1p( x ); } );
  auto ms = std::chrono::duration<double, std::milli>( std::chrono::steady_clock::now() - t0 ).count();
  std::cerr << name << ": " << ms << " ms\n";
  return r;
}

int main() {
  std::vector<double> v( 4'000'000 );
  std::iota( v.begin(), v.end(), 1.0 );
  double a = run( std::execution::seq, v, "seq" );
  double b = run( std::execution::par, v, "par" );
  std::cout << "results equal: " << ( std::abs( a - b ) / a < 1e-9 ) << "\n";   // reduction order differs slightly
}
