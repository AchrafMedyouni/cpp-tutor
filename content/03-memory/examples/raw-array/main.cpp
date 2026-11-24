#include <iostream>

constexpr int taille{ 5 };

int main() {
  int tab[taille];                    // automatic array: values uninitialised
  for (int i = 0; i < taille; ++i) tab[i] = 10 * i;
  std::cout << "auto:";
  for (int i = 0; i < taille; ++i) std::cout << " " << tab[i];
  std::cout << "\n";

  int n = 8;                          // known only at run time
  auto fib( new int[n] );             // int*
  fib[0] = fib[1] = 1;
  for (int i = 2; i < n; ++i) fib[i] = fib[i - 1] + fib[i - 2];
  std::cout << "dynamic:";
  for (int i = 0; i < n; ++i) std::cout << " " << fib[i];
  std::cout << "\n";
  delete[] fib;                       // delete[] for new[]
}
