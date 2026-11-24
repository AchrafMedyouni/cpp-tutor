#include <iostream>
#include <stdexcept>
#include <utility>

class Tableau {
public:
  explicit Tableau( unsigned taille = 100 ) : taille_{ taille }, tab_{ new double[taille]{} } {}
  ~Tableau() { delete[] tab_; }

  Tableau( const Tableau & tab ) : taille_{ tab.taille_ }, tab_{ new double[tab.taille_] } {
    for( unsigned i = 0; i < taille_; ++i ) tab_[i] = tab.tab_[i];
  }

  Tableau & operator=( const Tableau & tab ) {
    Tableau tmp{ tab };    // le constructeur de copie est utilisé pour faire la copie
    swap( tmp );
    return *this;
  }

  void swap( Tableau & tab ) {
    std::swap( taille_, tab.taille_ );
    std::swap( tab_   , tab.tab_ );
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
  const unsigned taille{ 10 };
  Tableau tab{ taille };
  for( unsigned i = 0; i < taille; ++i ) tab.get( i ) = i;

  Tableau tab2{ tab };
  Tableau tab3{ 2 };
  tab3 = tab;                 // OK, plus de double destruction, plus de fuite de mémoire
  std::cout << "tab2.get(3) = " << tab2.get( 3 ) << ", tab3.get(3) = " << tab3.get( 3 ) << "\n";
  tab3.get( 3 ) = -1;
  std::cout << "after tab3.get(3) = -1: tab2.get(3) = " << tab2.get( 3 ) << "\n";
  Tableau & same{ tab2 };
  tab2 = same;                // self-assignment through an alias
  std::cout << "self-assignment ok: " << tab2.get( 3 ) << "\n";
}
