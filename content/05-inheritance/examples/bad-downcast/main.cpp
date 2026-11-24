#include <iostream>

struct Point { int x{}, y{}; };
class Forme {
public:
  explicit Forme( Point c ) : centre_{ c } {}
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
  Forme* forme = new Forme{ { 1, 1 } };     // a plain Forme
  Cercle* pc = (Cercle *) forme;            // C cast: the compiler believes you
  std::cout << pc->rayon() << "\n";         // reads past the end of the Forme object
  delete forme;
}
