#pragma once

class Forme { public: virtual ~Forme() = default; };
class Cercle : public Forme {
public:
  explicit Cercle(double r) : r_{ r } {}
  double rayon() const { return r_; }
private:
  double r_;
};
class Rectangle : public Forme {};
