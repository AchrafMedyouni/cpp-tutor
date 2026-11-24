#include <iostream>

double spread(double bid, double ask);  // declared ...

int main() {
  std::cout << spread(99.5, 100.5) << "\n";  // ... used ...
}
// ... but never defined anywhere: the linker cannot resolve 'spread'.
