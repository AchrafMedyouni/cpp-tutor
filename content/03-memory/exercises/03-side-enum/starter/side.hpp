#pragma once
#include <stdexcept>
#include <string>

enum class Side { Buy, Sell };

inline const char* to_string(Side s) { (void)s; return ""; }        // TODO
inline Side opposite(Side s) { return s; }                          // TODO
inline int sign(Side s) { (void)s; return 0; }                      // TODO
inline Side parse_side(const std::string& text) { (void)text; return Side::Buy; }   // TODO
