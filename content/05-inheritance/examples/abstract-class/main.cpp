#include <iostream>

class Forme {
public:
  virtual ~Forme() = default;
  virtual void dessine() const = 0;   // pure virtual
};

class Cercle : public Forme {
public:
  void dessine() const override { std::cout << "()\n"; }
};

int main() {
  Cercle c;
  c.dessine();
  Forme f{};                          // error: Forme is abstract
}
