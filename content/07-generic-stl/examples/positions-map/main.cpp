#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct Fill { std::string symbol; long qty; double price; };   // qty < 0 for sells
struct Position { long qty = 0; double cost = 0; };             // cost of the long inventory

int main() {
  std::vector<Fill> fills{ { "AAPL", 100, 185.0 }, { "NVDA", 10, 950.0 }, { "AAPL", 50, 190.0 }, { "MSFT", -20, 410.0 } };
  std::map<std::string, Position> positions;                    // sorted by symbol
  for( const Fill & f : fills ) {
    Position & p = positions[f.symbol];                         // default-constructed if absent
    p.qty += f.qty;
    if( f.qty > 0 ) p.cost += f.qty * f.price;
  }
  std::cout << std::fixed << std::setprecision(2);
  for( const auto & [symbol, p] : positions )                   // structured bindings (C++17)
    std::cout << symbol << "  qty " << p.qty << "  avg " << ( p.qty > 0 ? p.cost / p.qty : 0.0 ) << "\n";
}
