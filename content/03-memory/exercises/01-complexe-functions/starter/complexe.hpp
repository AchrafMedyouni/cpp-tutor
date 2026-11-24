#pragma once
#include <cmath>

struct Complexe {
  double real;
  double imag;
};

inline double module(Complexe c) { (void)c; return 0; }                       // TODO
inline Complexe conjugate(Complexe c) { return c; }                           // TODO
inline Complexe add(Complexe a, Complexe b) { (void)b; return a; }            // TODO
inline bool approx_equal(Complexe a, Complexe b, double eps = 1e-9) {         // TODO
  (void)a; (void)b; (void)eps;
  return false;
}
