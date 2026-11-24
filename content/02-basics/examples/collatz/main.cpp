#include <iostream>

int main() {
  int n{};
  std::cin >> n;
  for( ;; ) {
    std::cout << n << "\n";
    if( n == 1 ) break;
    if( n % 2 == 0 ) { n /= 2; continue; }
    n = 3 * n + 1;
  }
}
