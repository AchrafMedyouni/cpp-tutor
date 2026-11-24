#include <iostream>
#include <stdexcept>

class Tableau {
public:
  Tableau( unsigned taille ) : taille_{ taille }, tab_{ new double[taille] } {}
  ~Tableau() { delete[] tab_; }
  Tableau( const Tableau & tab ) : taille_{ tab.taille_ }, tab_{ new double[tab.taille_] } {
    for( unsigned i = 0; i < taille_; ++i ) tab_[i] = tab.tab_[i];
  }
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
  Tableau tab2{ tab };   // OK: deep copy
  tab2 = tab;            // default operator= copies the pointer: tab2's buffer leaks, tab's is shared
  std::cout << "assigned\n";
}                        // fuite mémoire, double destruction
