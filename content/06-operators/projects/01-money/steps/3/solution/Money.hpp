#pragma once
#include <cmath>
#include <compare>
#include <cstdlib>
#include <iomanip>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

class Money {
public:
  Money(long cents, std::string currency) : cents_{ cents }, ccy_{ std::move(currency) } {}
  long cents() const { return cents_; }
  const std::string& currency() const { return ccy_; }

  Money& operator+=(const Money& m) { check(m); cents_ += m.cents_; return *this; }
  Money& operator-=(const Money& m) { check(m); cents_ -= m.cents_; return *this; }
  Money& operator*=(long k) { cents_ *= k; return *this; }
  Money operator-() const { return { -cents_, ccy_ }; }

  bool operator==(const Money&) const = default;
  std::strong_ordering operator<=>(const Money& m) const { check(m); return cents_ <=> m.cents_; }

private:
  void check(const Money& m) const {
    if (m.ccy_ != ccy_) throw std::invalid_argument{ "currency mismatch" };
  }
  long cents_;
  std::string ccy_;
};

inline Money operator+(Money a, const Money& b) { return a += b; }
inline Money operator-(Money a, const Money& b) { return a -= b; }
inline Money operator*(Money a, long k) { return a *= k; }
inline Money operator*(long k, Money a) { return a *= k; }

inline std::ostream& operator<<(std::ostream& os, const Money& m) {
  long a = std::labs(m.cents());
  if (m.cents() < 0) os << '-';
  return os << a / 100 << '.' << std::setw(2) << std::setfill('0') << a % 100 << std::setfill(' ') << ' ' << m.currency();
}

inline std::istream& operator>>(std::istream& is, Money& m) {   // "12.34 EUR", "-0.5 USD", "7 JPY"
  std::string amount, ccy;
  if (!(is >> amount >> ccy)) return is;
  bool neg = !amount.empty() && amount[0] == '-';
  std::string digits = neg ? amount.substr(1) : amount;
  auto dot = digits.find('.');
  std::string units = digits.substr(0, dot), frac = dot == std::string::npos ? "" : digits.substr(dot + 1);
  bool ok = !units.empty() && frac.size() <= 2 && ccy.size() == 3;
  for (char c : units + frac) ok = ok && c >= '0' && c <= '9';
  if (!ok) {
    is.setstate(std::ios::failbit);
    return is;
  }
  while (frac.size() < 2) frac += '0';
  long cents = std::stol(units) * 100 + std::stol(frac);
  m = Money{ neg ? -cents : cents, ccy };
  return is;
}

// Splits into n parts that differ by at most one cent and add up exactly (the first parts get the extra cents).
inline std::vector<Money> split(const Money& m, int n) {
  if (n <= 0) throw std::invalid_argument{ "n must be positive" };
  long base = m.cents() / n, rest = m.cents() % n;
  std::vector<Money> parts;
  for (int i = 0; i < n; ++i) parts.emplace_back(base + (i < std::labs(rest) ? (rest > 0 ? 1 : -1) : 0), m.currency());
  return parts;
}

// Applies a rate in basis points (1 bp = 0.01%), rounding half away from zero to the cent.
inline Money apply_bps(const Money& m, double bps) {
  return Money{ std::lround(m.cents() * bps / 10000.0), m.currency() };
}
