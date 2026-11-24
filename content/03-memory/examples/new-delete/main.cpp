#include <iostream>

struct Quote { double price; int size; };

int main() {
  int *p{ new int{} };
  *p = 5;
  std::cout << "*p = " << *p << "\n";
  delete p;                          // memory released

  Quote *q{ new Quote{ 101.25, 300 } };   // any type works
  std::cout << "quote: " << q->price << " x " << q->size << "\n";
  delete q;
}
