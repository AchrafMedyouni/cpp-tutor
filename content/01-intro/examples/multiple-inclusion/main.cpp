// Slide 21: this file plays the role of C.cpp.
#include "C.hpp"   // pastes A.hpp, then class C
#include "B.hpp"   // pastes A.hpp AGAIN → class A defined twice

int main() {
  B b;
  C c;
  (void)b; (void)c;
}
