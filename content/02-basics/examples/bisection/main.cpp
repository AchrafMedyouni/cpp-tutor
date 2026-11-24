#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>

// Finds x in [lo, hi] with f(x) == 0, assuming f(lo) and f(hi) have opposite signs.
double bisect(std::function<double(double)> f, double lo, double hi, double tol = 1e-10) {
  while (hi - lo > tol) {
    double mid = (lo + hi) / 2;
    if ((f(lo) < 0) == (f(mid) < 0)) lo = mid;
    else hi = mid;
  }
  return (lo + hi) / 2;
}

// 5-year bond, 5% annual coupon, face 100.
double bond_price(double y) {
  double p = 0;
  for (int t = 1; t <= 5; ++t) p += 5.0 / std::pow(1 + y, t);
  return p + 100.0 / std::pow(1 + y, 5);
}
double minus_95(double y) { return bond_price(y) - 95.0; }

int main() {
  std::cout << std::fixed << std::setprecision(4);
  std::cout << "price at 5%: " << bond_price(0.05) << "\n";
  std::cout << "yield for price 95: " << 100 * bisect(minus_95, 0.0, 1.0) << "%\n";
}
