#pragma once
#include <stdexcept>
#include <string>

enum class Side { Buy, Sell };

inline const char* to_string(Side s) { return s == Side::Buy ? "BUY" : "SELL"; }
inline Side opposite(Side s) { return s == Side::Buy ? Side::Sell : Side::Buy; }
inline int sign(Side s) { return s == Side::Buy ? 1 : -1; }
inline Side parse_side(const std::string& text) {
  if (text == "BUY" || text == "B") return Side::Buy;
  if (text == "SELL" || text == "S") return Side::Sell;
  throw std::invalid_argument{ "bad side: " + text };
}
