#include <iostream>

template< typename T, unsigned TAILLE = 100 >
class Tableau {
public:
  T & operator[]( unsigned i ) { return tab_[i]; }
  static constexpr unsigned taille() { return TAILLE; }
private:
  T tab_[TAILLE]{};          // stored inside the object: no heap allocation
};

int main() {
  Tableau< int > a;          // TAILLE defaults to 100
  Tableau< double, 3 > last; // a tiny ring of closes
  last[0] = 101; last[1] = 102; last[2] = 103;
  std::cout << "sizes: " << a.taille() << " " << last.taille() << "\n";
  std::cout << "sizeof(Tableau<double, 3>) = " << sizeof( last ) << "\n";
  std::cout << "last 3 closes: " << last[0] << " " << last[1] << " " << last[2] << "\n";
}
