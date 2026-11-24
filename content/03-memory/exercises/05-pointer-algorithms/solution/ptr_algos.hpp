#pragma once

inline void reverse(int* first, int* last) {
  while (first < last) {
    --last;
    int tmp = *first;
    *first = *last;
    *last = tmp;
    ++first;
  }
}

inline int* find_first(int* first, int* last, int value) {
  for (; first != last; ++first)
    if (*first == value) return first;
  return last;
}

inline long sum(const int* first, const int* last) {
  long s = 0;
  for (; first != last; ++first) s += *first;
  return s;
}

inline const int* max_pos(const int* first, const int* last) {
  if (first == last) return last;
  const int* best = first;
  for (++first; first != last; ++first)
    if (*first > *best) best = first;
  return best;
}
