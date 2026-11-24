#pragma once
#include <stdexcept>
#include <utility>

template< class T >
class Tableau {
public:
  Tableau() : Tableau( 1 ) {}            // non-explicit default constructor: new T[n]{} needs it
  explicit Tableau( unsigned taille ) : taille_{ taille }, tab_{ new T[taille]{} } {}
  ~Tableau() { delete[] tab_; }
  Tableau( const Tableau & t ) : taille_{ t.taille_ }, tab_{ new T[t.taille_] } {
    for( unsigned i = 0; i < taille_; ++i ) tab_[i] = t.tab_[i];
  }
  Tableau & operator=( const Tableau & t ) {
    Tableau tmp{ t };
    std::swap( taille_, tmp.taille_ );
    std::swap( tab_, tmp.tab_ );
    return *this;
  }

  T & operator[]( unsigned indice );
  unsigned taille() const { return taille_; }

private:
  unsigned taille_;
  T * tab_;
};

// Defined outside the class: still in the header (slide 147)
template< typename T >
T & Tableau< T >::operator[]( unsigned indice ) {
  if( indice >= taille_ ) throw std::out_of_range{ "Tableau<T>::operator[]" };
  return tab_[indice];
}
