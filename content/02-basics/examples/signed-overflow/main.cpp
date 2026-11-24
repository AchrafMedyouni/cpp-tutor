#include <iostream>

int main() {
  int x;
  std::cin >> x;               // 2147483647 == INT_MAX
  unsigned u = 4294967295u;    // UINT_MAX
  std::cout << "u + 1 = " << u + 1 << " (wraps: defined)\n";
  std::cout << "x + 1 = " << x + 1 << " (signed overflow: UB)\n";
}
