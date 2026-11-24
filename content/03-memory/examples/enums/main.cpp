#include <iostream>

enum class Color { red, green, blue };
enum Niveau { nul, moyen = 10, bon = 15 };   // C-style
enum class Side { Buy, Sell };

const char* to_string(Side s) { return s == Side::Buy ? "BUY" : "SELL"; }

int main() {
  Color c{ Color::green };
  std::cout << "green -> " << static_cast<int>(c) << "\n";   // explicit conversion

  Niveau n{ moyen };       // no need for Niveau::
  int a{ n };              // implicit conversion
  std::cout << "moyen -> " << a << "\n";

  Side s{ Side::Sell };
  std::cout << to_string(s) << "\n";
}
