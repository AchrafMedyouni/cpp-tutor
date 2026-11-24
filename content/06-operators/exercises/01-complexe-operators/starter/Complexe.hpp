#pragma once

class Complexe {
public:
  Complexe(double re = 0, double im = 0) : re_{ re }, im_{ im } {}
  double real() const { return re_; }
  double imag() const { return im_; }
  // TODO: operators
private:
  double re_, im_;
};
