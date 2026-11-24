#include <iostream>

struct Complexe { double real; double imag; };

int main() {
  int i{ 1 }, j{ 9 };
  int       * const a{ &i };   // constant pointer
  int const *       b{ &i };   // pointer to constant
  *a = 2;                      // ok: the pointee is modifiable
  // a = &j;                   // error: a itself is const
  b = &i;                      // ok: b itself is not const
  // *b = 3;                   // error: can't modify through b
  (void)j;
  std::cout << "i = " << i << ", *b = " << *b << "\n";

  Complexe c{ 1, 2 };
  Complexe *pc{ &c };
  ( *pc ).real = 2.0;          // parentheses required
  pc->imag = -32.84321;        // same thing, nicer
  std::cout << "(*pc).real = " << (*pc).real << ", pc->imag = " << pc->imag << "\n";
}
