#pragma once
#include <typeinfo>
#include <vector>
#include "formes.hpp"

inline int count_cercles(const std::vector<const Forme*>& v) {
  int n = 0;
  for (const Forme* f : v)
    if (dynamic_cast<const Cercle*>(f)) ++n;
  return n;
}

inline double sum_rayons(const std::vector<const Forme*>& v) {
  double s = 0;
  for (const Forme* f : v)
    if (auto* c = dynamic_cast<const Cercle*>(f)) s += c->rayon();
  return s;
}

inline bool is_cercle(const Forme& f) {
  try {
    (void)dynamic_cast<const Cercle&>(f);
    return true;
  } catch (const std::bad_cast&) {
    return false;
  }
}
