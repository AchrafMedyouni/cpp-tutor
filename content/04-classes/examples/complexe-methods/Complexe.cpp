#include "Complexe.hpp"
#include <cmath>

double Complexe::module() const {
  // Complexe const * const this : modification des attributs interdite
  return std::sqrt( real * real + imag * imag );
}

double Complexe::module_explicit_this() const {
  return std::sqrt( this->real * this->real + this->imag * this->imag );
}
