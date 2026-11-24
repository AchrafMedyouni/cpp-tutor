#pragma once
#include <algorithm>
#include <cstddef>
#include <map>
#include <string>
#include <utility>
#include <vector>

struct Fill { std::string symbol; long qty; double price; };

inline std::map<std::string, double> vwap_by_symbol(const std::vector<Fill>& fills) {
  std::map<std::string, std::pair<double, long>> acc;   // notional, volume
  for (const Fill& f : fills) {
    auto& [notional, volume] = acc[f.symbol];
    notional += f.qty * f.price;
    volume += f.qty;
  }
  std::map<std::string, double> out;
  for (const auto& [sym, nv] : acc) out[sym] = nv.first / nv.second;
  return out;
}

inline std::vector<std::string> most_traded(const std::vector<Fill>& fills, std::size_t k) {
  std::map<std::string, long> vol;
  for (const Fill& f : fills) vol[f.symbol] += f.qty;
  std::vector<std::pair<std::string, long>> v(vol.begin(), vol.end());
  std::sort(v.begin(), v.end(), [](const auto& a, const auto& b) {
    return a.second != b.second ? a.second > b.second : a.first < b.first;
  });
  std::vector<std::string> out;
  for (std::size_t i = 0; i < v.size() && i < k; ++i) out.push_back(v[i].first);
  return out;
}
