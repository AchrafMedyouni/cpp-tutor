#include <iostream>

int max( int a, int b ) {
  std::cout << "max(int, int) -> ";
  return a > b ? a : b;
}
double max( double a, double b ) {
  std::cout << "max(double, double) -> ";
  return a > b ? a : b;
}

int main() {
  std::cout << max( 3, 7 ) << "\n";       // exact match: int version
  std::cout << max( 2.5, 1.5 ) << "\n";   // exact match: double version
  std::cout << max( static_cast<double>( 1 ), 2.5 ) << "\n";
}
