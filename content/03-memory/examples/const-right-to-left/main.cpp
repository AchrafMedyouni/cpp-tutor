#include <iostream>

int main() {
  int x = 1, y = 2;

  const int* p = &x;          // read right to left: p is a pointer to a const int
  p = &y;                     // ✓ the pointer itself can be re-aimed
  // *p = 5;                  // ✗ can't write THROUGH p (it's a read-only view)
  x = 10;                     // ✓ x itself is not const: p doesn't freeze it

  int* const q = &x;          // q is a const pointer to an int
  *q = 20;                    // ✓ the pointee can be modified
  // q = &y;                  // ✗ q will point to x forever

  const int* const r = &x;    // const pointer to const int: neither can change
  // *r = 1;                  // ✗
  // r = &y;                  // ✗

  const int* view = q;        // ✓ adding const is always allowed
  // int* sneaky = p;         // ✗ removing const never is

  std::cout << "x = " << x << ", *p = " << *p << ", *r = " << *r << ", *view = " << *view << '\n';
}
