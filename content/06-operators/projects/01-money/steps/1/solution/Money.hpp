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
