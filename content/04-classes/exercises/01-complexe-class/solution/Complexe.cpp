#include "Complexe.hpp"
#include <cmath>

double Complexe::module() const { return std::sqrt(real_ * real_ + imag_ * imag_); }
