#include <iostream>

class Forme {
public:
  void dessine() const { std::cout << "Forme::dessine\n"; }
};
class Cercle : public Forme {
public:
  void dessine() const { std::cout << "Cercle::dessine\n"; }   // hides, does not override
};

int main() {
  Cercle c;
  Forme* pf = &c;
  std::cout << "c.dessine():   "; c.dessine();
  std::cout << "pf->dessine(): "; pf->dessine();   // chosen at compile time from Forme*
}
