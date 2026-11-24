#pragma once
#include <stdexcept>

class Ema {
public:
  explicit Ema(double alpha) : alpha_{ alpha } {
    if (!(alpha > 0 && alpha <= 1)) throw std::invalid_argument{ "alpha must be in (0, 1]" };
  }
  double operator()(double x) {
    value_ = started_ ? alpha_ * x + (1 - alpha_) * value_ : x;
    started_ = true;
    return value_;
  }
  double value() const { return value_; }
private:
  double alpha_;
  double value_ = 0;
  bool started_ = false;
};
