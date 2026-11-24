#include <iostream>

int main() {
  int i{ 1 }, j{ 2 };

  // i par valeur, j par référence
  auto sum = [i, &j]() { return i + j; };

  // Affiche 3
  std::cout << sum() << "\n";

  ++i; ++j;
  // Affiche 4
  std::cout << sum() << "\n";
}
