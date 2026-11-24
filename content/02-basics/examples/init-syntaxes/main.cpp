#include <initializer_list>
#include <iostream>
#include <type_traits>

int main() {
  int j{ 5 };
  int k{};            // zero
  auto l{ 2021ul };   // unsigned long
  auto f = -2.129e-3f;
  auto v = { 1 };     // std::initializer_list<int> !

  std::cout << "j = " << j << ", k = " << k << "\n";
  std::cout << "l = " << l << " (unsigned long: " << std::is_same_v<decltype(l), unsigned long> << ")\n";
  std::cout << "f = " << f << " (float: " << std::is_same_v<decltype(f), float> << ")\n";
  std::cout << "v is an initializer_list: "
            << std::is_same_v<decltype(v), std::initializer_list<int>> << "\n";
}
