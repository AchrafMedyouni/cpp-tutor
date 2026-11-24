#include <algorithm>
#include <compare>
#include <iomanip>
#include <iostream>
#include <vector>

struct Version {
  int major, minor;
  auto operator<=>( const Version & ) const = default;
};

struct Bid {
  int id;
  long price_ticks;   // price in hundredths, as an integer
  int time;
  // Better priority = higher price, then earlier time. "a < b" means "a goes first".
  std::strong_ordering operator<=>( const Bid & o ) const {
    if( auto c = o.price_ticks <=> price_ticks; c != 0 ) return c;   // reversed: higher price first
    return time <=> o.time;                                          // earlier first
  }
  bool operator==( const Bid & o ) const { return price_ticks == o.price_ticks && time == o.time; }
};

int main() {
  std::cout << "1.2 < 1.10 ? " << ( Version{ 1, 2 } < Version{ 1, 10 } ) << "\n";
  std::cout << "1.10 == 1.10 ? " << ( Version{ 1, 10 } == Version{ 1, 10 } ) << "\n";

  std::vector<Bid> bids{ { 1, 10050, 1 }, { 2, 9975, 2 }, { 3, 10100, 5 }, { 4, 10050, 7 } };
  std::sort( bids.begin(), bids.end() );        // uses operator<, synthesised from <=>
  std::cout << "bids by priority:\n" << std::fixed << std::setprecision(2);
  for( const Bid & b : bids )
    std::cout << "  #" << b.id << " " << b.price_ticks / 100.0 << " @ t=" << b.time << "\n";
}
