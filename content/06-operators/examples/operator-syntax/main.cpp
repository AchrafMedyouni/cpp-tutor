#include <iostream>
#include <string>

int main() {
  std::string a{ "EUR" }, b{ "/USD" };
  std::cout << a + b << "\n";
  std::cout << operator+( a, b ) << "\n";          // the free function std::operator+
  std::string c = a;
  c.operator+=( b );                               // the member std::string::operator+=
  std::cout << "size via member call: " << c.size() << "\n";
  std::cout.operator<<( 42 ) << "\n";              // std::ostream::operator<<(int) is a member
}
