#include <iostream>
#include <utility>
#include <vector>

class Tableau {
public:
  inline static int copies = 0, moves = 0;
  explicit Tableau( unsigned taille = 0 ) : taille_{ taille }, tab_{ new double[taille]{} } {}
  ~Tableau() { delete[] tab_; }
  Tableau( const Tableau & t ) : taille_{ t.taille_ }, tab_{ new double[t.taille_] } {
    ++copies;
    for( unsigned i = 0; i < taille_; ++i ) tab_[i] = t.tab_[i];
  }
  Tableau & operator=( const Tableau & t ) { Tableau tmp{ t }; swap( tmp ); return *this; }

  Tableau( Tableau && tab ) noexcept      // construction par déplacement
    : taille_{ tab.taille_ }
    , tab_   { tab.tab_    } {
    ++moves;
    tab.tab_ = nullptr;
    tab.taille_ = 0;
  }
  Tableau & operator=( Tableau && tab ) noexcept {   // affectation par déplacement
    ++moves;
    swap( tab );
    return *this;
  }

  void swap( Tableau & t ) noexcept { std::swap( taille_, t.taille_ ); std::swap( tab_, t.tab_ ); }
  unsigned taille() const { return taille_; }
  static void reset() { copies = moves = 0; }
private:
  unsigned taille_;
  double * tab_;
};

Tableau make_series( unsigned n ) {
  Tableau t{ n };
  return t;            // no copy: elided or moved
}

int main() {
  Tableau::reset();
  Tableau t1 = make_series( 1000 );
  std::cout << "make_series: " << Tableau::copies << " copies, " << Tableau::moves << " moves (return value elided)\n";

  Tableau t2;
  t2 = std::move( t1 );
  std::cout << "t2 = std::move(t1): t1.taille() = " << t1.taille() << ", t2.taille() = " << t2.taille() << "\n";

  Tableau::reset();
  std::vector<Tableau> v;
  for( int i = 0; i < 17; ++i ) v.emplace_back( 10 );   // reallocations relocate the elements
  std::cout << "vector growth with noexcept moves: " << Tableau::copies << " copies, " << Tableau::moves << " moves\n";
}
