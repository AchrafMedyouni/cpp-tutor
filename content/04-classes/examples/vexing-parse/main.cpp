#include <iostream>

class Tableau {
public:
  Tableau( unsigned taille = 100 ) : taille_{ taille } {}
  unsigned taille() const { return taille_; }
private:
  unsigned taille_;
};

int main() {
  Tableau t1( 10 );
  Tableau t2{ 10 };
  Tableau t3();          // a FUNCTION declaration, not an object!
  Tableau t4{};
  Tableau t5;
  Tableau tab[10];       // requires a default constructor
  std::cout << t1.taille() << " " << t2.taille() << " " << t4.taille() << " " << t5.taille() << " " << tab[0].taille() << "\n";
  // std::cout << t3.taille() << "\n";
}
