#pragma once
#include <stdexcept>
#include <utility>

class Tableau {
public:
  explicit Tableau(unsigned taille) : taille_{ taille }, tab_{ new double[taille]{} } {}
  double& get(unsigned indice) {
    if (indice >= taille_) throw std::out_of_range{ "Tableau::get" };
    return tab_[indice];
  }
  unsigned taille() const { return taille_; }

  ~Tableau() { delete[] tab_; }

private:
  unsigned taille_;
  double* tab_;
};
