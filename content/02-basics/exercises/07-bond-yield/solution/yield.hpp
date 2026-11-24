#pragma once
#include <cmath>
#include <functional>

inline double bond_price(double face, double coupon_rate, int years, double y) {
  double p = 0;
  for (int t = 1; t <= years; ++t) p += coupon_rate * face / std::pow(1 + y, t);
  return p + face / std::pow(1 + y, years);
}

inline double bisect(std::function<double(double)> f, double lo, double hi, double tol = 1e-10) {
  double flo = f(lo);
  while (hi - lo > tol) {
    double mid = (lo + hi) / 2;
    double fmid = f(mid);
    if ((flo < 0) == (fmid < 0)) {
      lo = mid;
      flo = fmid;
    } else {
      hi = mid;
    }
  }
  return (lo + hi) / 2;
}

inline double yield_to_maturity(double price, double face, double coupon_rate, int years) {
  return bisect([&](double y) { return bond_price(face, coupon_rate, years, y) - price; }, 0.0, 1.0);
}
