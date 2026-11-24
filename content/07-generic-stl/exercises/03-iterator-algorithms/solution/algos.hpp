#pragma once
#include <cstddef>

template <typename It, typename F>
void for_each_(It first, It last, F f) {
  for (; first != last; ++first) f(*first);
}

template <typename It, typename P>
std::size_t count_if_(It first, It last, P pred) {
  std::size_t n = 0;
  for (; first != last; ++first)
    if (pred(*first)) ++n;
  return n;
}

template <typename It, typename T>
T sum_(It first, It last, T init) {
  for (; first != last; ++first) init = init + *first;
  return init;
}

template <typename It, typename P>
It find_if_(It first, It last, P pred) {
  for (; first != last; ++first)
    if (pred(*first)) return first;
  return last;
}
