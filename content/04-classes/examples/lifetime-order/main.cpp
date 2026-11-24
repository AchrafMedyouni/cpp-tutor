#include <iostream>
#include <string>

struct Noisy {
  std::string name;
  explicit Noisy(std::string n) : name{ std::move(n) } { std::cout << "+ " << name << "\n"; }
  ~Noisy() { std::cout << "- " << name << "\n"; }
};

Noisy g{ "global" };   // static storage: built before main, destroyed after

int main() {
  std::cout << "main starts\n";
  Noisy a{ "a" };
  Noisy b{ "b" };
  {
    Noisy inner{ "inner" };   // destroyed at the end of this block
  }
  Noisy* h = new Noisy{ "heap" };
  delete h;
  std::cout << "main ends\n";
}   // automatic objects are destroyed in REVERSE order of construction: b, then a
