#pragma once
#include <string>

inline int max3(int a, int b) { return a > b ? a : b; }

inline int max3(int a, int b, int c) { return max3(max3(a, b), c); }

inline double max3(double a, double b, double c) {
  double m = a > b ? a : b;
  return m > c ? m : c;
}

inline std::string max3(const std::string& a, const std::string& b, const std::string& c) {
  const std::string& m = a > b ? a : b;
  return m > c ? m : c;
}
