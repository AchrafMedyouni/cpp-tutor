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

  Tableau(const Tableau& t) : taille_{ t.taille_ }, tab_{ new double[t.taille_] } {
    for (unsigned i = 0; i < taille_; ++i) tab_[i] = t.tab_[i];
  }

private:
  unsigned taille_;
  double* tab_;
};
