#include <iostream>

struct Point { int x{}, y{}; };

class Forme {
public:
  explicit Forme( Point centre ) : centre_{ centre } { std::cout << "Forme(Point)\n"; }
  Forme( const Forme & f ) : centre_{ f.centre_ } { std::cout << "Forme(const Forme&)\n"; }
  ~Forme() { std::cout << "~Forme\n"; }
  void deplace( Point nouveau_centre ) {
    centre_ = nouveau_centre;
    std::cout << "Forme::deplace -> (" << centre_.x << ", " << centre_.y << ")\n";
  }
protected:
  Point centre_;
};

class Cercle : public Forme {
public:
  Cercle( Point centre, int rayon )
    : Forme{ centre }, rayon_{ rayon }       // base first
  { std::cout << "Cercle(Point, int)\n"; }
  Cercle( const Cercle & c )
    : Forme{ c }, rayon_{ c.rayon_ }          // c binds to const Forme&
  { std::cout << "Cercle(const Cercle&)\n"; }
  ~Cercle() { std::cout << "~Cercle\n"; }
  void dessine() const {
    std::cout << "c at (" << centre_.x << ", " << centre_.y << "), r = " << rayon_ << "\n";   // centre_ is protected: OK
  }
private:
  int rayon_;
};

int main() {
  Cercle c{ { 1, 2 }, 3 };
  c.dessine();
  c.deplace( { 5, 5 } );    // inherited method
  c.dessine();
  std::cout << "Copy:\n";
  Cercle c2{ c };
}
