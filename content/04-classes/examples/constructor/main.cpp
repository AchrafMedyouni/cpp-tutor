#include <iostream>

class Complexe {
public:
  Complexe( double real, double imag ) {   // same name as the class, no return type
    this->real = real;
    this->imag = imag;
  }
  Complexe() { real = 0; imag = 0; }       // overload: no argument
  explicit Complexe( double r ) { real = r; imag = 0; }

  void print() const { std::cout << "(" << real << ", " << imag << ")\n"; }

private:
  double real;
  double imag;
};

int main() {
  Complexe c{ 2, -5 };
  Complexe zero;          // needs Complexe()
  Complexe r{ 7.0 };
  c.print();
  zero.print();
  r.print();
}
