#include <iostream>

int trace(const char* name, int v) {
  std::cout << name << " ";
  return v;
}

int sum(int a, int b) { return a + b; }

int main() {
  int tab[3] = { 0, 0, 0 };
  int i = 0;
  tab[i] = i++;      // C++17: i++ first (yields 0, i becomes 1), then tab[1] = 0
  std::cout << "after tab[i] = i++: i = " << i << ", tab = {" << tab[0] << ", " << tab[1] << ", " << tab[2] << "}\n";

  std::cout << "arguments evaluated: ";
  int s = sum(trace("left", 1), trace("right", 2));   // order unspecified!
  std::cout << "-> " << s << "\n";

  std::cout << "<< chain is left to right since C++17: ";
  std::cout << trace("a", 1) << trace("b", 2) << "\n";
}
