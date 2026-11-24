#pragma once
#include <stdexcept>

class Price {
public:
  explicit Price(double v) : v_{ v } {
    if (v < 0) throw std::invalid_argument{ "negative price" };
  }
  double value() const { return v_; }
private:
  double v_;
};

class Quantity {
public:
  explicit Quantity(long q) : q_{ q } {}
  long value() const { return q_; }
private:
  long q_;
};

inline double notional(Price p, Quantity q) { return p.value() * q.value(); }
