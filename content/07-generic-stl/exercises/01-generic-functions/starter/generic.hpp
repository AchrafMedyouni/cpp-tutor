#pragma once
#include <cstddef>
#include <stdexcept>
#include <vector>

template <typename T>
T max_of(const std::vector<T>& v) { return v.at(0); }   // TODO

template <typename T>
T clamp_to(T v, T lo, T hi) { (void)lo; (void)hi; return v; }   // TODO

template <typename T>
std::size_t count_equal(const std::vector<T>& v, const T& x) { (void)v; (void)x; return 0; }   // TODO
