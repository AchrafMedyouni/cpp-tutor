#pragma once
#include <cstdlib>
#include <iomanip>
#include <ostream>
#include <stdexcept>
#include <string>

class Money {
public:
  Money(long cents, std::string currency) : cents_{ cents }, ccy_{ std::move(currency) } {}
  long cents() const { return cents_; }
  const std::string& currency() const { return ccy_; }

  Money& operator+=(const Money& m) { check(m); cents_ += m.cents_; return *this; }
  Money& operator-=(const Money& m) { check(m); cents_ -= m.cents_; return *this; }
  Money& operator*=(long k) { cents_ *= k; return *this; }

  bool operator==(const Money&) const = default;
  bool operator<(const Money& m) const { check(m); return cents_ < m.cents_; }

private:
  void check(const Money& m) const {
    if (m.ccy_ != ccy_) throw std::invalid_argument{ "currency mismatch: " + ccy_ + " vs " + m.ccy_ };
  }
  long cents_;
  std::string ccy_;
};

inline Money operator+(Money a, const Money& b) { return a += b; }
inline Money operator-(Money a, const Money& b) { return a -= b; }
inline Money operator*(Money a, long k) { return a *= k; }
inline Money operator*(long k, Money a) { return a *= k; }

inline std::ostream& operator<<(std::ostream& os, const Money& m) {
  long c = m.cents();
  if (c < 0) os << '-';
  long a = std::labs(c);
  return os << a / 100 << '.' << std::setw(2) << std::setfill('0') << a % 100 << std::setfill(' ') << ' ' << m.currency();
}
