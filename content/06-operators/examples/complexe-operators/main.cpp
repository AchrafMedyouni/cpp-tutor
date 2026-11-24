#include <iostream>

class Complexe {
public:
  Complexe( double re = 0, double im = 0 ) : re_{ re }, im_{ im } {}   // also converts double → Complexe
  double real() const { return re_; }
  double imag() const { return im_; }

  Complexe & operator+=( const Complexe & c ) {   // member: modifies *this
    re_ += c.re_;
    im_ += c.im_;
    return *this;
  }
  Complexe operator-() const { return { -re_, -im_ }; }   // unary minus

  friend Complexe operator*( const Complexe & c1, const Complexe & c2 ) {
    return { c1.re_ * c2.re_ - c1.im_ * c2.im_, c1.re_ * c2.im_ + c1.im_ * c2.re_ };
  }

private:
  double re_, im_;
};

Complexe operator+( const Complexe & c1, const Complexe & c2 ) {   // free, built from +=
  Complexe res{ c1 };
  return res += c2;
}

void print( const char* label, const Complexe & c ) {
  std::cout << label << " = (" << c.real() << ", " << c.imag() << ")\n";
}

int main() {
  Complexe a{ 1, 2 }, b{ 3, -1 };
  print( "a + b", a + b );
  print( "a * b", a * b );
  print( "-a", -a );
  print( "2.0 + a", 2.0 + a );   // works because + is a free function
  a += b;
  std::cout << "a += b → (" << a.real() << ", " << a.imag() << ")\n";
}
