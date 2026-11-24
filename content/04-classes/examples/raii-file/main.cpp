#include <fstream>
#include <iostream>
#include <string>

int main() {
  {
    std::ofstream out{ "trades.csv" };   // constructor opens the file
    out << "AAPL,100,189.5\n" << "MSFT,50,410.2\n" << "NVDA,10,950\n";
    std::cout << "wrote 3 trades\n";
  }                                       // destructor flushes and closes it

  std::ifstream in{ "trades.csv" };
  std::string line;
  std::cout << "read back:";
  bool first = true;
  while (std::getline(in, line)) {
    std::cout << (first ? " " : " | ") << line;
    first = false;
  }
  std::cout << "\n";
}                                         // in is closed here
