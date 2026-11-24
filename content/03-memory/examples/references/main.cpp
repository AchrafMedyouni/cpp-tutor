#include <iostream>

int main() {
  int   i{ 3 };     // auto   i{ 3 };
  // i est une valeur gauche
  int & ri{ i };    // auto & ri{ i };

  i  =  5;  std::cout << "i = " << i << ", ri = " << ri << "\n";   // i == ri == 5
  ri = -2;  std::cout << "i = " << i << ", ri = " << ri << "\n";   // i == ri == -2

  int j{ 7 };
  ri = j;   std::cout << "ri = j  → i = " << i << "\n";   // assigns j's VALUE to i: ri still aliases i
  j = -1;   std::cout << "j = -1 → i = " << i << "\n";    // i unchanged: 7

  //     int & rk{ 8 };  // 8 n'est pas une lvalue
  const int & ck{ 8 };   // OK: the temporary's lifetime is extended
  std::cout << "ck = " << ck << "\n";
}
