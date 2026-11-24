#include <iostream>

int main() {
  int filled{ 5 }, orders{ 2 };
  int bad{ filled / orders };                            // integer division: 2
  double d{ static_cast<double>(filled) / orders };      // 2.5
  std::cout << "integer mean: " << bad << "\n";
  std::cout << "double mean: " << d << "\n";
}
