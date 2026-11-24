#pragma once
#include "vendor_a.hpp"
#include "vendor_b.hpp"

namespace desk {
inline double best_bid(double spot) {
  double a = vendor_a::bid(spot);
  double b = vendor_b::bid(spot);
  return a > b ? a : b;
}
}  // namespace desk

namespace va = vendor_a;
