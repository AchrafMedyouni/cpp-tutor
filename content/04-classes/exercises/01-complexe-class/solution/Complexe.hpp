#pragma once

class Complexe {
public:
  Complexe() = default;
  Complexe(double re, double im) : real_{ re }, imag_{ im } {}

  double real() const { return real_; }
  void real(double v) { real_ = v; }
  double imag() const { return imag_; }
  void imag(double v) { imag_ = v; }

  double module() const;
  void ajoute(const Complexe& other) {
    real_ += other.real_;
    imag_ += other.imag_;
  }

private:
  double real_ = 0;
  double imag_ = 0;
};
