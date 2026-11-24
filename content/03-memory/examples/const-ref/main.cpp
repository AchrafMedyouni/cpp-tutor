#include <iostream>
#include <string>
#include <vector>

void f4( const int & i ) {
  int k{ i };
  // i = 3;
  std::cout << k << " ";
}

// Big objects: pass by const reference, no copy and no modification.
void print_all(const std::vector<std::string>& tickers) {
  std::cout << "portfolio of " << tickers.size() << " tickers:";
  for (const std::string& t : tickers) std::cout << " " << t;
  std::cout << "\n";
}

int main() {
  f4( 5 );
  int j{ 8 };
  f4( j );
  f4( j + 2 );
  std::cout << "\n";
  print_all({ "AAPL", "MSFT", "NVDA" });
}
