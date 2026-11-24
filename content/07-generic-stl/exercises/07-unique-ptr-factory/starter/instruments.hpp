#pragma once
#include <string>

struct Instrument {
  virtual ~Instrument() = default;
  virtual std::string kind() const = 0;
  virtual double price() const = 0;
};

struct Bond : Instrument {
  double face, coupon;
  Bond(double f, double c) : face{ f }, coupon{ c } {}
  std::string kind() const override { return "BOND"; }
  double price() const override { return face * (1 + coupon); }   // toy model
};

struct Equity : Instrument {
  double last;
  explicit Equity(double l) : last{ l } {}
  std::string kind() const override { return "EQUITY"; }
  double price() const override { return last; }
};

inline int& instruments_destroyed() { static int n = 0; return n; }

struct Tracked : Equity {          // lets the tests check that everything gets destroyed
  using Equity::Equity;
  ~Tracked() override { ++instruments_destroyed(); }
};
