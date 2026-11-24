#include <iostream>

int main() {
  int i{ 5 };                 // une variable de type entier
  int *p{ &i };               // p "pointe" sur i
  std::cout << "i = " << i << ", &i = " << &i << ", p = " << p << ", *p = " << *p << "\n";

  *p = -7;                    // modification de la valeur de i
  std::cout << "after *p = -7: i = " << i << "\n";

  int ** q{ &p };             // pointer to pointer
  **q = 3;
  std::cout << "after **q = 3: i = " << i << "\n";

  std::cout << "&*p == p ? " << (&*p == p) << ", *&i == i ? " << (*&i == i) << "\n";

  int *n{ nullptr };          // points to nothing
  std::cout << "n is null: " << (n == nullptr) << "\n";
}
