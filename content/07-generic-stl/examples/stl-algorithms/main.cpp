#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

int main() {
  std::vector<double> close{ 100.0, 101.5, 101.0, 104.5, 104.0, 98.0, 100.0 };

  auto hi = std::max_element( close.begin(), close.end() );
  auto lo = std::min_element( close.begin(), close.end() );
  std::cout << "max close " << *hi << " on day " << hi - close.begin() << "\n";
  std::cout << "min close " << *lo << " on day " << lo - close.begin() << "\n";

  // adjacent pairs: count the up days
  int up = 0;
  for( std::size_t i = 1; i < close.size(); ++i ) up += close[i] > close[i - 1];
  std::cout << "up days: " << up << "\n";
  std::cout << "all positive: " << std::all_of( close.begin(), close.end(), []( double p ) { return p > 0; } ) << "\n";

  double mean = std::accumulate( close.begin(), close.end(), 0.0 ) / close.size();
  std::cout << "mean: " << mean << "\n";

  std::vector<double> ret( close.size() - 1 );
  std::transform( close.begin() + 1, close.end(), close.begin(), ret.begin(),
                  []( double today, double yday ) { return 100 * ( today / yday - 1 ); } );
  std::cout << "returns %:" << std::fixed << std::setprecision(2);
  for( double r : ret ) std::cout << " " << r;
  std::cout << std::defaultfloat << std::setprecision(6) << "\n";

  std::vector<double> sorted = close;
  std::sort( sorted.begin(), sorted.end() );
  std::cout << "sorted:";
  for( double p : sorted ) std::cout << " " << p;
  std::cout << "\nfirst >= 103: " << *std::lower_bound( sorted.begin(), sorted.end(), 103.0 ) << "\n";
}
