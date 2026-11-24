// Slide 25, verbatim apart from comments.
#include <iostream>

int main() {
  int n;
  std::cin >> n;
  if( std::cin.good() ) {
    std::cout << "carré de " << n
              << " = " << n * n
              << std::endl;
  }
  return 0;
}
