#include <iostream>
#include <string>
#include "Tableau.hpp"

int main() {
  Tableau< int >                tab1{ 20 };
  Tableau< std::string >        tab2{ 30 };
  Tableau< Tableau< double >>   tab3{ 10 };     // each element is a Tableau<double>{} (default size 1)

  for( unsigned i = 0; i < tab1.taille(); ++i ) tab1[i] = static_cast<int>( i );
  tab2[0] = "AAPL";
  // tab3[2] = tab2;
  tab3[2] = Tableau< double >{ 8 };
  tab3[2][6] = 3.0;

  std::cout << "tab1[19] = " << tab1[19] << "\n";
  std::cout << "tab2[0] = " << tab2[0] << "\n";
  std::cout << "tab3[2][6] = " << tab3[2][6] << "\n";
  std::cout << "tab3[2].taille() = " << tab3[2].taille() << "\n";
}
