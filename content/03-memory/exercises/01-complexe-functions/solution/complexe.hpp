#pragma once
#include <cmath>

struct Complexe {
  double real;
  double imag;
};

inline double module(Complexe c) { return std::sqrt(c.real * c.real + c.imag * c.imag); }
inline Complexe conjugate(Complexe c) { return { c.real, -c.imag }; }
inline Complexe add(Complexe a, Complexe b) { return { a.real + b.real, a.imag + b.imag }; }
inline bool approx_equal(Complexe a, Complexe b, double eps = 1e-9) {
  return std::fabs(a.real - b.real) <= eps && std::fabs(a.imag - b.imag) <= eps;
}
