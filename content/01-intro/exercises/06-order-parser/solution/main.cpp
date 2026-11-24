#include <iomanip>
#include <iostream>
#include <string>

int main() {
  long position = 0;
  double cash = 0.0;
  std::string side;
  int k = 0;
  while (std::cin >> side) {
    ++k;
    long qty;
    double price;
    if ((side != "BUY" && side != "SELL") || !(std::cin >> qty >> price)) {
      std::cout << "error: bad order #" << k << "\n";
      break;
    }
    if (side == "BUY") {
      position += qty;
      cash -= qty * price;
    } else {
      position -= qty;
      cash += qty * price;
    }
  }
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "position: " << position << "\n";
  std::cout << "cash: " << cash << "\n";
}
