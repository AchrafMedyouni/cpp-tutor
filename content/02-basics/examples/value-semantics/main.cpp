#include <iostream>
#include <string>

int main() {
  int a{ 1 };
  int b{ a };   // b gets a copy of a's value
  b = 2;        // a is unaffected
  std::cout << "a = " << a << ", b = " << b << "\n";

  std::string s1{ "AAPL" };
  std::string s2{ s1 };   // a full, independent copy of the characters
  s2 += ".O";
  std::cout << "s1 = \"" << s1 << "\", s2 = \"" << s2 << "\"\n";
}
