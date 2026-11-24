#include <functional>
#include <iomanip>
#include <iostream>
#include <queue>
#include <vector>

struct Bid { double price; long size; };
struct ByPrice { bool operator()( const Bid & a, const Bid & b ) const { return a.price < b.price; } };   // max-heap on price

int main() {
  std::priority_queue<Bid, std::vector<Bid>, ByPrice> bids;
  bids.push( { 100.10, 500 } );
  bids.push( { 100.25, 300 } );
  bids.push( { 99.90, 1000 } );
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "best bid: " << bids.top().price << " x " << bids.top().size << "\n";
  bids.pop();
  std::cout << "after matching it: " << bids.top().price << " x " << bids.top().size << "\n";

  // asks: the LOWEST price is best, so use std::greater for a min-heap
  std::priority_queue<double, std::vector<double>, std::greater<double>> asks;
  for( double a : { 100.55, 100.30, 100.40 } ) asks.push( a );
  std::cout << "asks, best first:";
  while( !asks.empty() ) { std::cout << " " << asks.top(); asks.pop(); }
  std::cout << "\n";
}
