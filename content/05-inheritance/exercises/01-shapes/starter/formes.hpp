#pragma once
#include <vector>

class Forme {
public:
  // TODO: virtual destructor, pure virtual aire() and perimetre()
};

class Cercle : public Forme {
public:
  explicit Cercle(double r) : r_{ r } {}
private:
  double r_;
};

class Rectangle : public Forme {
public:
  Rectangle(double l, double h) : l_{ l }, h_{ h } {}
private:
  double l_, h_;
};

// TODO: class Carre

inline double total_area(const std::vector<const Forme*>& formes) { (void)formes; return 0; }   // TODO
