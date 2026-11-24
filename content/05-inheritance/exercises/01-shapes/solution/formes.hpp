#pragma once
#include <vector>

class Forme {
public:
  virtual ~Forme() = default;
  virtual double aire() const = 0;
  virtual double perimetre() const = 0;
};

class Cercle : public Forme {
public:
  explicit Cercle(double r) : r_{ r } {}
  double aire() const override { return 3.14159265358979 * r_ * r_; }
  double perimetre() const override { return 2 * 3.14159265358979 * r_; }
private:
  double r_;
};

class Rectangle : public Forme {
public:
  Rectangle(double l, double h) : l_{ l }, h_{ h } {}
  double aire() const override { return l_ * h_; }
  double perimetre() const override { return 2 * (l_ + h_); }
private:
  double l_, h_;
};

class Carre : public Forme {
public:
  explicit Carre(double c) : c_{ c } {}
  double aire() const override { return c_ * c_; }
  double perimetre() const override { return 4 * c_; }
private:
  double c_;
};

inline double total_area(const std::vector<const Forme*>& formes) {
  double t = 0;
  for (const Forme* f : formes) t += f->aire();
  return t;
}
