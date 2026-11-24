#include <iostream>
#include <string>

struct Complexe {
  double real;
  double imag;
};

struct Trade {                 // a finance aggregate
  std::string symbol;
  bool buy;
  int quantity;
  double price;
};

int main() {
  Complexe c{ 1.3, -5.2 };
  std::cout << "c = (" << c.real << ", " << c.imag << ")\n";
  double r = c.real;           // read
  c.imag += 2;                 // write
  std::cout << "r = " << r << ", c.imag = " << c.imag << "\n";

  Complexe c_nul{ 0, 0 };
  c = c_nul;                   // assignment copies every member
  std::cout << "after c = c_nul: (" << c.real << ", " << c.imag << ")\n";

  Trade t{ "AAPL", true, 100, 189.5 };
  std::cout << t.symbol << (t.buy ? " BUY " : " SELL ") << t.quantity << " @ " << t.price << "\n";
}
