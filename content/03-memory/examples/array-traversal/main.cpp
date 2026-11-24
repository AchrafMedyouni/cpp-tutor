#include <iostream>

void print(const int* t, int n) {
  for (int i = 0; i < n; ++i) std::cout << t[i] << (i + 1 < n ? " " : "\n");
}

int main() {
  int tab[10];
  for( int i{}; i < 10; ++i ) {               // par indice
    tab[i] = 0;
  }
  print(tab, 10);
  for( int * p{ tab }; p < tab + 10; ++p ) {  // par pointeur
    *p = 7;
  }
  print(tab, 10);
}
