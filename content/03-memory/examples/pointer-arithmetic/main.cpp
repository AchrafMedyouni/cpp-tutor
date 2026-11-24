#include <cstddef>
#include <iostream>

void f(const int* a, int n) {          // an "array parameter" is just a pointer
  std::cout << "inside f: n = " << n << ", sizeof(a) = " << sizeof(a) << "\n";
}

int main() {
  int tab[10];
  for (int i = 0; i < 10; ++i) tab[i] = i * i;
  int * p1{ &tab[0] };
  int * p2{ &tab[5] };
  std::ptrdiff_t i{ p2 - p1 };         // distance in ELEMENTS
  std::cout << "p2 - p1 = " << i << "\n";
  p2 = p1 + i;
  p1 = p2 - i;
  std::cout << "*(tab + 3) == tab[3] ? " << (*(tab + 3) == tab[3]) << "\n";
  std::cout << "sizeof(tab) = " << sizeof(tab) << ", sizeof(p1) = " << sizeof(p1) << "\n";
  f(tab, 10);                          // tab decays to &tab[0]: the size must be passed separately
  const char* b1 = reinterpret_cast<const char*>(p1);
  const char* b2 = reinterpret_cast<const char*>(p1 + 1);
  std::cout << "bytes between p1 and p1 + 1: " << (b2 - b1) << "\n";
}
