#include <iostream>

int main() {
  int nombre{};
  std::cin >> nombre;
  std::cout << nombre << " est un nombre "
            << (( nombre % 2 == 0 ) ?
                "pair" : "impair" )
            << "\n";
}
