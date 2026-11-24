#pragma once
#include <compare>
#include <numeric>
#include <ostream>
#include <stdexcept>

class Fraction {
public:
  Fraction(long num, long den = 1) : num_{ num }, den_{ den } {
    if (den_ == 0) throw std::invalid_argument{ "zero denominator" };
    if (den_ < 0) { num_ = -num_; den_ = -den_; }
    long g = std::gcd(num_, den_);
    if (g > 1) { num_ /= g; den_ /= g; }
  }
  long num() const { return num_; }
  long den() const { return den_; }

  bool operator==(const Fraction&) const = default;   // normalised: member-wise equality is exact
  std::strong_ordering operator<=>(const Fraction& o) const { return num_ * o.den_ <=> o.num_ * den_; }

private:
  long num_, den_;
};

inline Fraction operator+(const Fraction& a, const Fraction& b) { return { a.num() * b.den() + b.num() * a.den(), a.den() * b.den() }; }
inline Fraction operator-(const Fraction& a, const Fraction& b) { return { a.num() * b.den() - b.num() * a.den(), a.den() * b.den() }; }
inline Fraction operator*(const Fraction& a, const Fraction& b) { return { a.num() * b.num(), a.den() * b.den() }; }
inline Fraction operator/(const Fraction& a, const Fraction& b) {
  if (b.num() == 0) throw std::invalid_argument{ "division by zero" };
  return { a.num() * b.den(), a.den() * b.num() };
}

inline std::ostream& operator<<(std::ostream& os, const Fraction& f) {
  os << f.num();
  if (f.den() != 1) os << '/' << f.den();
  return os;
}
