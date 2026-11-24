#pragma once
#include <utility>

class Tableau {
public:
  explicit Tableau(unsigned taille = 0) : taille_{ taille }, tab_{ new double[taille]{} } {}
  ~Tableau() { delete[] tab_; }
  Tableau(const Tableau& t) : taille_{ t.taille_ }, tab_{ new double[t.taille_] } {
    for (unsigned i = 0; i < taille_; ++i) tab_[i] = t.tab_[i];
  }
  Tableau& operator=(const Tableau& t) { Tableau tmp{ t }; swap(tmp); return *this; }

  // TODO: move constructor and move assignment

  void swap(Tableau& t) noexcept { std::swap(taille_, t.taille_); std::swap(tab_, t.tab_); }
  unsigned taille() const { return taille_; }
  double& operator[](unsigned i) { return tab_[i]; }
  const double* data() const { return tab_; }
private:
  unsigned taille_;
  double* tab_;
};
