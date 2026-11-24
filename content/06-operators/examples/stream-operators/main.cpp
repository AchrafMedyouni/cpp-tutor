#include <iostream>

class Complexe {
public:
  Complexe( double re = 0, double im = 0 ) : re_{ re }, im_{ im } {}
  friend Complexe operator*( const Complexe & a, const Complexe & b ) {
    return { a.re_ * b.re_ - a.im_ * b.im_, a.re_ * b.im_ + a.im_ * b.re_ };
  }
  friend std::ostream & operator<<( std::ostream & out, const Complexe & c );
  friend std::istream & operator>>( std::istream & in, Complexe & c );
private:
  double re_, im_;
};

std::ostream & operator<<( std::ostream & out, const Complexe & c ) {
  return out << "(" << c.re_ << "," << c.im_ << ")";
}

std::istream & operator>>( std::istream & in, Complexe & c ) {   // format: (re,im)
  char open, comma, close;
  double re, im;
  if( in >> open >> re >> comma >> im >> close && open == '(' && comma == ',' && close == ')' )
    c = Complexe{ re, im };
  else
    in.setstate( std::ios::failbit );
  return in;
}

int main() {
  Complexe c;
  while( std::cin >> c ) {
    std::cout << "carré de " << c << " = " << c * c << std::endl;
  }
  std::cout << "stopped: bad input\n";
}
