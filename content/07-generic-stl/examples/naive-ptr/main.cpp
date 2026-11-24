#include <iostream>

template< typename T >
class ptr {
public:
  ptr( T * p ) : p_{ p } {}
  ~ptr() { delete p_; }

  T * operator ->() { return  p_; }
  T & operator  *() { return *p_; }
private:
  T * p_;
};

struct Forme  { virtual ~Forme() = default; virtual double aire() const = 0; };
struct Cercle : Forme {
  double r;
  explicit Cercle( double r ) : r{ r } {}
  ~Cercle() override { std::cout << "~Cercle\n"; }
  double aire() const override { return 3.14159265358979 * r * r; }
};

int main() {
  {
    ptr< int   > p1{ new int{ 1 }};
    ptr< Forme > f1{ new Cercle{ 2 }};
    std::cout << "*p1 = " << *p1 << "\n";
    std::cout << "f1->aire() = " << f1->aire() << "\n";
  }   // both deleted here
  std::cout << "(no delete needed)\n";
}
