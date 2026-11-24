#pragma once
#include <stdexcept>

class Tableau {
public:
  explicit Tableau(unsigned taille) : taille_{ taille }, tab_{ new double[taille]{} } {}
  ~Tableau() { delete[] tab_; }
  Tableau(const Tableau&) = delete;
  Tableau& operator=(const Tableau&) = delete;
  unsigned taille() const { return taille_; }

  double& operator[](unsigned i) {
    if (i >= taille_) throw std::out_of_range{ "Tableau::operator[]" };
    return tab_[i];
  }
  const double& operator[](unsigned i) const {
    if (i >= taille_) throw std::out_of_range{ "Tableau::operator[]" };
    return tab_[i];
  }
  double sum() const {
    double s = 0;
    for (unsigned i = 0; i < taille_; ++i) s += (*this)[i];
    return s;
  }

private:
  unsigned taille_;
  double* tab_;
};
