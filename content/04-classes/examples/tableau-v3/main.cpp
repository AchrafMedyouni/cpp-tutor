#include <iostream>
#include <stdexcept>

class Tableau {
public:
  Tableau( unsigned taille ) : taille_{ taille }, tab_{ new double[taille] } {}
  ~Tableau() { delete[] tab_; }

  Tableau( const Tableau & tab ) {
    taille_ = tab.taille_;
    tab_ = new double[tab.taille_];                 // a NEW buffer
    for( unsigned i = 0; i < tab.taille_; ++i ) {   // slide: tab.taille (typo)
      tab_[i] = tab.tab_[i];                        // copy the elements
    }
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
  Tableau tab2{ tab };   // deep copy
  tab2.get( 0 ) = 42;    // independent
  std::cout << "tab.get(0) = " << tab.get( 0 ) << ", tab2.get(0) = " << tab2.get( 0 ) << "\n";
}                        // two buffers, two deletes: OK
