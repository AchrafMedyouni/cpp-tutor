// The slide-13 example (written in Python there), in C++.
// Each call to bar() gets its own stack frame: a, b, c live there.
#include <iostream>

int bar(int a, int b) {
  int c = a + b;
  std::cout << "  bar(" << a << ", " << b << "): frame of bar at " << &c << "\n";
  return c * 2;
}

int foo() {
  int x = 3;
  std::cout << "foo: x lives at " << &x << "\n";
  std::cout << bar(x, x + 2) << "\n";  // prints 16
  return bar(7, x + 5);                // returns 30
}

int main() {
  std::cout << "foo() returned " << foo() << "\n";
}
