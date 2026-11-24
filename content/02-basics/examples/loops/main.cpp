#include <iostream>

int main() {
  int n{};
  do {                       // ask until positive (slide 39)
    std::cin >> n;
  } while (n <= 0 && std::cin);

  int res{ 1 }, k{ n };
  while (k > 0) {            // slide 39
    res = res * k;
    --k;
  }
  std::cout << n << "! = " << res << "\n";

  for (int i = 1; i <= 5; ++i) {
    if (i != 3) continue;    // skip to the next iteration
    std::cout << "skipping " << i << "\n";
  }
  for (int i = 51;; ++i) {   // no condition: must break out
    if (i % 7 == 0) { std::cout << "first multiple of 7 above 50: " << i << "\n"; break; }
  }
}
