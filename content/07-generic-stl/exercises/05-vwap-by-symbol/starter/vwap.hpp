#pragma once
#include <algorithm>
#include <cstddef>
#include <map>
#include <string>
#include <utility>
#include <vector>

struct Fill { std::string symbol; long qty; double price; };

inline std::map<std::string, double> vwap_by_symbol(const std::vector<Fill>& fills) { (void)fills; return {}; }   // TODO
inline std::vector<std::string> most_traded(const std::vector<Fill>& fills, std::size_t k) { (void)fills; (void)k; return {}; }   // TODO
