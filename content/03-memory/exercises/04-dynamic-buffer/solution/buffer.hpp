#pragma once

inline double* make_prices(int n, double start, double step) {
  double* p = new double[n];
  for (int i = 0; i < n; ++i) p[i] = start + i * step;
  return p;
}

inline double average(const double* p, int n) {
  double s = 0;
  for (int i = 0; i < n; ++i) s += p[i];
  return n > 0 ? s / n : 0;
}

inline double* grow(double* old, int old_n, int new_n) {
  double* p = new double[new_n];
  for (int i = 0; i < new_n; ++i) p[i] = i < old_n ? old[i] : 0.0;
  delete[] old;
  return p;
}

inline void release(double* p) { delete[] p; }
