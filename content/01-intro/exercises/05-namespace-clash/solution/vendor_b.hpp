#pragma once
#include <string>

namespace vendor_b {
inline double bid(double spot) { return spot * 0.9999; }
inline std::string name() { return "Vendor B"; }
}  // namespace vendor_b
