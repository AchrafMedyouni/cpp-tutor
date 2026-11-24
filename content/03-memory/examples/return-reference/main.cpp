#include <iostream>

int & getVal() {
  static int val{ 5 };
  return val;              // val outlives the call
}

struct Book { double levels[3]; };

double& level(Book& b, int i) { return b.levels[i]; }   // returns an lvalue

int main() {
  std::cout << getVal() << "\n";
  getVal() = 8;            // the call itself is an lvalue
  std::cout << getVal() << "\n";

  Book b{ { 100.0, 100.5, 101.0 } };
  level(b, 1) = 101.5;     // assign through the returned reference
  std::cout << "book[1] = " << b.levels[1] << "\n";
}
