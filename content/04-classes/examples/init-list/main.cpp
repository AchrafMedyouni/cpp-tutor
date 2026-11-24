#include <iostream>
#include <string>

class Tableau {
public:
  Tableau( unsigned taille )
    : taille_{ taille }
    , tab_{ new double[taille] } {
    // Rien
  }
  ~Tableau() { delete[] tab_; }
  Tableau( const Tableau & ) = delete;            // keep the example short
  Tableau & operator=( const Tableau & ) = delete;
  double & get( unsigned i ) { return tab_[i]; }
  unsigned taille() const { return taille_; }
private:
  unsigned int const taille_;
  double * const     tab_;      // the POINTER is const; the doubles are not
};

class Order {
public:
  Order(long id, std::string symbol, int qty) : id_{ id }, symbol_{ std::move(symbol) }, qty_{ qty } {}
  void print() const { std::cout << "order: " << symbol_ << " x " << qty_ << " (id " << id_ << ")\n"; }
private:
  const long id_;               // an order never changes id
  std::string symbol_;
  int qty_;
};

int main() {
  Tableau t{ 5 };
  for (unsigned i = 0; i < t.taille(); ++i) t.get(i) = i;
  std::cout << "taille " << t.taille() << ", last " << t.get(4) << "\n";
  Order o{ 7, "AAPL", 100 };
  o.print();
}
