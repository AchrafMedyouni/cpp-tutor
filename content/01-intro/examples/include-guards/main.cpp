#include <iostream>
#include "C.hpp"
#include "B.hpp"   // A.hpp's body is skipped the second time

int main() {
  B b;
  C c;
  (void)b; (void)c;
  std::cout << "compiled: A was defined only once\n";
}
