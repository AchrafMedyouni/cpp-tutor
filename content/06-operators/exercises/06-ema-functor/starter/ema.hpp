#pragma once
#include <stdexcept>

class Ema {
public:
  explicit Ema(double alpha) : alpha_{ alpha } {}
  // TODO: operator(), value()
private:
  double alpha_;
};
