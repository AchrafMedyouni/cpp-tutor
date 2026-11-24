#include <iostream>
#include <stdexcept>

class Tableau {
public:
  Tableau( unsigned taille ) {
    std::cout << "constructing Tableau(" << taille << ")\n";
    taille_ = taille;
    tab_ = new double[taille];
  }

  ~Tableau() {
    std::cout << "~Tableau(): releasing " << taille_ << " doubles\n";
    delete[] tab_;
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
  Tableau t{ 3 };
  for( unsigned i = 0; i < t.taille(); ++i ) t.get( i ) = i * i;
  std::cout << "t.get(2) = " << t.get( 2 ) << "\n";
}   // the destructor runs here, automatically
