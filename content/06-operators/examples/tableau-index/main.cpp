#include <iostream>
#include <stdexcept>

class Tableau {
public:
  explicit Tableau( unsigned taille ) : taille_{ taille }, tab_{ new double[taille]{} } {}
  ~Tableau() { delete[] tab_; }
  Tableau( const Tableau & ) = delete;
  Tableau & operator=( const Tableau & ) = delete;

  double & operator[]( unsigned indice ) {
    if( indice < taille_ ) return tab_[indice];
    throw std::out_of_range{ "Tableau::operator[]" };
  }
  const double & operator[]( unsigned indice ) const {   // used on const objects
    if( indice < taille_ ) return tab_[indice];
    throw std::out_of_range{ "Tableau::operator[]" };
  }
  unsigned taille() const { return taille_; }

private:
  unsigned int taille_;
  double *     tab_;
};

double sum( const Tableau & t ) {          // needs the const overload
  double s = 0;
  for( unsigned i = 0; i < t.taille(); ++i ) s += t[i];
  return s;
}

int main() {
  Tableau t{ 10 };
  for( unsigned i = 0; i < t.taille(); ++i ) t[i] = i * i;   // writes through the reference
  std::cout << "sum = " << sum( t ) << "\n";
  try { t[10] = 1; }
  catch( const std::out_of_range & e ) { std::cout << "out of range: " << e.what() << "\n"; }
}
