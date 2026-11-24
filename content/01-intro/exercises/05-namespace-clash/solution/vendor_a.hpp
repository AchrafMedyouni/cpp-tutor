#pragma once
#include <string>

namespace vendor_a {
inline double bid(double spot) { return spot - 0.02; }
inline std::string name() { return "Vendor A"; }
}  // namespace vendor_a
