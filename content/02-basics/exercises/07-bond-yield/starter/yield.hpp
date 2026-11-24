#pragma once
#include <cmath>
#include <functional>

inline double bond_price(double face, double coupon_rate, int years, double y) {
  (void)coupon_rate; (void)years; (void)y;
  return face;   // TODO
}

inline double bisect(std::function<double(double)> f, double lo, double hi, double tol = 1e-10) {
  (void)f; (void)hi; (void)tol;
  return lo;     // TODO
}

inline double yield_to_maturity(double price, double face, double coupon_rate, int years) {
  (void)price; (void)face; (void)coupon_rate; (void)years;
  return 0;      // TODO
}
