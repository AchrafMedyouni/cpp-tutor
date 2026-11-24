#include <iostream>

int main() {
  const unsigned IOC       = 1u << 0;   // immediate-or-cancel
  const unsigned POST_ONLY = 1u << 1;
  const unsigned REDUCE    = 1u << 2;   // reduce-only

  unsigned flags = IOC | REDUCE;        // set two bits
  std::cout << "flags = " << flags << "\n";
  std::cout << "IOC? " << ((flags & IOC) != 0) << "\n";
  std::cout << "POST_ONLY? " << ((flags & POST_ONLY) != 0) << "\n";
  flags &= ~IOC;                        // clear a bit
  std::cout << "after clearing IOC: " << flags << "\n";
}
