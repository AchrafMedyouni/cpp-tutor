#include <iostream>

struct Point { int x{}, y{}; };

class Forme {
public:
  explicit Forme( Point c ) : centre_{ c } {}
  Point centre() const { return centre_; }
  const char* nom() const { return "Forme"; }
protected:
  Point centre_;
};

class Cercle : public Forme {
public:
  Cercle( Point c, int r ) : Forme{ c }, rayon_{ r } {}
  int rayon() const { return rayon_; }
private:
  int rayon_;
};

int main() {
  Point p1{ 0, 0 }, p2{ 9, 9 };
  Forme f{ p1 }, *pf{ &f };
  Cercle c{ p2, 3 }, *pc{ &c };
  f = c;                      // slicing: only centre_ is copied
  // c = f;
  std::cout << "f = c   -> f is a " << f.nom() << " centred at (" << f.centre().x << ", " << f.centre().y << ")\n";

  Cercle c2{ { 1, 1 }, 1 };
  pf = &c2;                   // upcast
  *pf = *pc;                  // Attention: Forme::operator= copies only the Forme part
  std::cout << "*pf = *pc -> c2: centre (" << c2.centre().x << ", " << c2.centre().y << "), rayon still " << c2.rayon() << "\n";
  (void)pf;
}
