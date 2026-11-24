#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
  std::vector< int > tab( 10 );   // Parenthèses ! 10 elements, all 0
  tab[5] = -2;                    // pas de vérification d'indice
  tab.at( 5 ) = -2;               // avec vérification d'indice
  std::cout << "size " << tab.size() << ", tab[5] = " << tab[5] << "\n";
  tab.resize( 20 );
  std::cout << "after resize: " << tab.size() << "\n";
  try {
    tab.at( 25 ) = 1;
  } catch (const std::out_of_range&) {
    std::cout << "at(25) threw: out_of_range\n";
  }

  std::vector<int> copy = tab;    // a real copy (value semantics)
  copy[0] = 99;
  std::cout << "copies are independent: " << tab[0] << " vs " << copy[0] << "\n";
  std::cout << "equal? " << (copy == tab) << "\n";   // element-wise comparison

  for( auto & el : tab ) el = 0;  // foreach, by reference to modify
}
