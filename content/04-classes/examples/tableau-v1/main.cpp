#include <iostream>
#include <stdexcept>

class Tableau {
public:
  Tableau( unsigned taille ) {
    taille_ = taille;
    tab_ = new double[taille];
  }
  double & get( unsigned indice ) {
    if( indice >= taille_ ) throw std::out_of_range{ "Tableau::get" };   // ... erreur
    return tab_[indice];
  }
  unsigned taille() const { return taille_; }

private:
  unsigned int taille_;
  double *     tab_;
};

double test1() {
  const unsigned taille{ 10 };
  Tableau tab{ taille };      // allocation d'une zone mémoire par le constructeur

  for( unsigned i = 0; i < taille; ++i ) {
    double d{};
    std::cin >> d;
    tab.get( i ) = d;
  }
  double sum = 0;
  for( unsigned i = 0; i < taille; ++i ) sum += tab.get( i );
  return sum;
}                             // pas de libération de la zone mémoire : fuite de mémoire !

int main() {
  std::cout << "sum = " << test1() << "\n";
}
