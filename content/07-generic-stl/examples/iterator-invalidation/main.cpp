#include <iostream>
#include <vector>

int main() {
  std::vector<double> fills{ 100.1, 100.3 };
  auto best = fills.begin();          // iterator to the first fill
  fills.push_back( 100.2 );           // may reallocate: best now points into freed memory
  std::cout << "best fill: " << *best << "\n";
}
