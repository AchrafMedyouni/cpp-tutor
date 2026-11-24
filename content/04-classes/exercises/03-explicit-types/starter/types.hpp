#pragma once
#include <stdexcept>

class Price {
public:
  Price(double v) : v_{ v } {}
  double value() const { return v_; }
private:
  double v_;
};

class Quantity {
public:
  Quantity(long q) : q_{ q } {}
  long value() const { return q_; }
private:
  long q_;
};

inline double notional(Price p, Quantity q) { (void)p; (void)q; return 0; }   // TODO
