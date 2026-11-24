#include <iomanip>
#include <iostream>

class Vwap {                     // volume-weighted average price
public:
  double operator()( double price, long qty ) {   // callable like a function...
    notional_ += price * qty;
    volume_ += qty;
    return notional_ / volume_;  // ...but remembers previous calls
  }
private:
  double notional_{ 0 };
  long volume_{ 0 };
};

int main() {
  Vwap vwap;
  std::cout << std::fixed << std::setprecision(3);
  std::cout << "vwap after fill 1: " << vwap( 100.0, 100 ) << "\n";
  std::cout << "vwap after fill 2: " << vwap( 102.0, 50 ) << "\n";
  std::cout << "vwap after fill 3: " << vwap( 100.25, 90 ) << "\n";
}
