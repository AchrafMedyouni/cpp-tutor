#include <iostream>
#include <typeinfo>

class Forme { public: virtual ~Forme() = default; };
class Cercle : public Forme {
public:
  explicit Cercle(int r) : r_{ r } {}
  int rayon() const { return r_; }
private:
  int r_;
};
class Rectangle : public Forme {};

void foo( Forme * f ) {
  Cercle * c2{ dynamic_cast< Cercle * >( f ) };   // avec vérification
  if( c2 ) std::cout << "circle of radius " << c2->rayon() << "\n";
  else std::cout << "not a circle\n";
}

void bar( Forme & f ) {
  try {
    Cercle & c{ dynamic_cast< Cercle & >( f ) };
    std::cout << c.rayon() << "\n";
  }
  catch( const std::bad_cast & ) {               // by const reference
    std::cout << "reference cast failed: std::bad_cast\n";
  }
}

int main() {
  Forme* a = new Cercle{ 2 };
  Forme* b = new Rectangle;
  foo( a );
  foo( b );
  bar( *b );
  std::cout << "typeid: " << (typeid(*b) == typeid(Rectangle) ? "Rectangle" : "?") << "\n";
  delete a;
  delete b;
}
