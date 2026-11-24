#include <cmath>
#include <iostream>

class ComplexeCartesian {
public:
  ComplexeCartesian(double re, double im) : real_{ re }, imag_{ im } {}
  double real() const { return real_; }
  double imag() const { return imag_; }
  double module() const { return std::hypot(real_, imag_); }
private:
  double real_, imag_;
};

class ComplexePolar {               // same public interface, different representation
public:
  ComplexePolar(double re, double im) : rho_{ std::hypot(re, im) }, theta_{ std::atan2(im, re) } {}
  double real() const { return rho_ * std::cos(theta_); }
  double imag() const { return rho_ * std::sin(theta_); }
  double module() const { return rho_; }
private:
  double rho_, theta_;
};

template <typename C>
void print(const C& c) {
  std::cout << "real = " << std::round(c.real() * 1e9) / 1e9 << ", imag = " << std::round(c.imag() * 1e9) / 1e9
            << ", module = " << c.module() << "\n";
}

int main() {
  print(ComplexeCartesian{ 3, 4 });
  print(ComplexePolar{ 3, 4 });
}
