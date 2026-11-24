#include <cstring>
#include <iostream>

struct Plain   { int x; void f() {} };
struct Virtual { int x; virtual void f() {} virtual ~Virtual() = default; };

struct Forme  { virtual ~Forme() = default; virtual void dessine() const {} };
struct Cercle : Forme { void dessine() const override {} };

int main() {
  std::cout << "sizeof(Plain)   = " << sizeof(Plain) << "\n";
  std::cout << "sizeof(Virtual) = " << sizeof(Virtual) << "\n";   // vptr (8) + int (4) + padding (4)
  Cercle a, b;
  void* vptr_a; void* vptr_b;
  std::memcpy(&vptr_a, static_cast<void*>(&a), sizeof(void*));   // peek at the hidden first word
  std::memcpy(&vptr_b, static_cast<void*>(&b), sizeof(void*));
  std::cout << "same vtable for two Cercle objects: " << (vptr_a == vptr_b) << "\n";
}
