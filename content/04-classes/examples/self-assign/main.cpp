#include <iostream>

class Complexe {
public:
  Complexe( double real, double imag ) {
    real = real;            // BUG: both names refer to the parameter
    imag = imag;
  }
  double re() const { return real; }
private:
  double real{};
  double imag{};
};

int main() {
  Complexe c{ 2, -5 };
  std::cout << c.re() << "\n";   // 0, not 2
}
