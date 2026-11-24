#include <cmath>
#include <iostream>
#include "Complexe.hpp"

int main() {
  Complexe c{ 3, 4 };
  std::cout << "|c| = " << c.module() << "\n";
  std::cout << "this-> form: " << c.module_explicit_this() << "\n";
  Complexe c2{ 1, 2 };
  c2.ajoute( c );                     // modifies c2
  std::cout << "after ajoute: (" << c2.real << ", " << c2.imag << ")\n";
  std::cout << "|c2| = " << std::round(c2.module() * 10000) / 10000 << "\n";
}
