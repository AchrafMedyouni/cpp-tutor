#include <iostream>

class Forme {
public:
  ~Forme() { std::cout << "~Forme\n"; }            // NOT virtual
  virtual int sommets() const { return 0; }
};

class Polygone : public Forme {
public:
  explicit Polygone( int n ) : n_{ n }, xs_{ new double[n] } {}
  ~Polygone() { std::cout << "~Polygone\n"; delete[] xs_; }   // never called below
  int sommets() const override { return n_; }
private:
  int n_;
  double* xs_;
};

int main() {
  Forme* f = new Polygone{ 5 };
  std::cout << f->sommets() << " sommets\n";
  delete f;                                          // only ~Forme runs
}
