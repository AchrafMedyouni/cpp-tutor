#pragma once

class Complexe {
public:
  Complexe(double re = 0, double im = 0) : re_{ re }, im_{ im } {}
  double real() const { return re_; }
  double imag() const { return im_; }

  Complexe& operator+=(const Complexe& c) { re_ += c.re_; im_ += c.im_; return *this; }
  Complexe& operator-=(const Complexe& c) { re_ -= c.re_; im_ -= c.im_; return *this; }
  Complexe& operator*=(const Complexe& c) {
    double re = re_ * c.re_ - im_ * c.im_;
    im_ = re_ * c.im_ + im_ * c.re_;
    re_ = re;
    return *this;
  }
  Complexe operator-() const { return { -re_, -im_ }; }
  bool operator==(const Complexe&) const = default;

private:
  double re_, im_;
};

inline Complexe operator+(const Complexe& a, const Complexe& b) { Complexe r{ a }; return r += b; }
inline Complexe operator-(const Complexe& a, const Complexe& b) { Complexe r{ a }; return r -= b; }
inline Complexe operator*(const Complexe& a, const Complexe& b) { Complexe r{ a }; return r *= b; }
