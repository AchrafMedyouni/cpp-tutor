#include <iostream>
#include <stdexcept>

class Tableau {
public:
  Tableau( unsigned taille ) : taille_{ taille }, tab_{ new double[taille] } {}
  ~Tableau() { delete[] tab_; }
  double & get( unsigned indice ) {
    if( indice >= taille_ ) throw std::out_of_range{ "Tableau::get" };
    return tab_[indice];
  }
  unsigned taille() const { return taille_; }
private:
  unsigned int taille_;
  double *     tab_;
};

int main() {
  Tableau tab{ 10 };
  for( unsigned i = 0; i < tab.taille(); ++i ) tab.get( i ) = i;

  Tableau tab2{ tab };   // comportement par défaut : recopie des attributs (the pointer!)
  std::cout << "tab2 shares tab's buffer\n";
}                        // double destruction
