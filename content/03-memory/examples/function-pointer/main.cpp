#include <functional>
#include <iostream>

// Déclaration (et définition) de la fonction
long partie_entiere( double d ) { return static_cast<long>( d ); }

int main() {
  // Définition du pointeur (compliqué !)
  long ( *pf )( double d );
  // Affectation autorisée
  pf = &partie_entiere;
  // Appel
  long d = ( *pf )( 3.5 );          // the slide stores it in a double
  std::cout << "(*pf)(3.5) = " << d << "\n";
  // Conversion implicite
  std::cout << "pf(2.8) = " << pf( 2.8 ) << "\n";
  // Compatibilité avec std::function
  std::function< long( double ) > foo( pf );
  std::cout << "foo(9.99) = " << foo( 9.99 ) << "\n";
}
