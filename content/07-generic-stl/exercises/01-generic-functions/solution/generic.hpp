#pragma once
#include <cstddef>
#include <stdexcept>
#include <vector>

template <typename T>
T max_of(const std::vector<T>& v) {
  if (v.empty()) throw std::invalid_argument{ "max_of: empty vector" };
  T best = v[0];
  for (const T& x : v)
    if (best < x) best = x;
  return best;
}

template <typename T>
T clamp_to(T v, T lo, T hi) { return v < lo ? lo : (hi < v ? hi : v); }

template <typename T>
std::size_t count_equal(const std::vector<T>& v, const T& x) {
  std::size_t n = 0;
  for (const T& y : v)
    if (y == x) ++n;
  return n;
}
