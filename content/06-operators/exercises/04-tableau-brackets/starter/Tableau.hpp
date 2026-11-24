#pragma once
#include <stdexcept>

class Tableau {
public:
  explicit Tableau(unsigned taille) : taille_{ taille }, tab_{ new double[taille]{} } {}
  ~Tableau() { delete[] tab_; }
  Tableau(const Tableau&) = delete;
  Tableau& operator=(const Tableau&) = delete;
  unsigned taille() const { return taille_; }
  // TODO: operator[] (two overloads), sum()
private:
  unsigned taille_;
  double* tab_;
};
