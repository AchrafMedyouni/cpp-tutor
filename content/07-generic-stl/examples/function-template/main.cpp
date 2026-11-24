#include <iostream>
#include <string>

class Complexe {
public:
  Complexe( double re, double im ) : re_{ re }, im_{ im } {}
  double real() const { return re_; }
  double imag() const { return im_; }
private:
  double re_, im_;
};

template< typename T >
inline T max( T val1, T val2 ) {
  return val1 > val2 ? val1 : val2;
}

template<>                                  // Complexe has no operator>
inline Complexe max( Complexe c1, Complexe c2 ) {
  return c1.real() == c2.real()
           ? c1.imag() > c2.imag() ? c1 : c2
           : c1.real() > c2.real() ? c1 : c2;
}

int main() {
  int i{ 5 }, j{ 3 }, k{ max( i, j ) };     // T = int
  std::cout << "max(5, 3) = " << k << "\n";
  // ::max — with std::string arguments, std::max is ALSO found (argument-dependent lookup): ambiguous otherwise
  std::cout << "max(\"bid\", \"ask\") = " << ::max( std::string{ "bid" }, std::string{ "ask" } ) << "\n";
  Complexe c1{ 1, 2 }, c2{ 1, 6 };
  Complexe c3{ max( c1, c2 ) };             // uses the specialisation
  std::cout << "max(c1, c2) = (" << c3.real() << ", " << c3.imag() << ")\n";
}
