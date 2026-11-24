#pragma once

inline double* make_prices(int n, double start, double step) {
  (void)n; (void)start; (void)step;
  return nullptr;   // TODO
}

inline double average(const double* p, int n) { (void)p; (void)n; return 0; }   // TODO

inline double* grow(double* old, int old_n, int new_n) {
  (void)old_n; (void)new_n;
  return old;       // TODO
}

inline void release(double* p) { (void)p; }   // TODO
