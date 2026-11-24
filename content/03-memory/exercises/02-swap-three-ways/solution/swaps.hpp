#pragma once

inline void swap_ptr(int* a, int* b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

inline void swap_ref(int& a, int& b) {
  int tmp = a;
  a = b;
  b = tmp;
}

inline void rotate3(int& a, int& b, int& c) {
  swap_ref(a, b);   // (b, a, c)
  swap_ref(b, c);   // (b, c, a)
}

inline void apply_fill(long& position, double& cash, long qty, double price) {
  position += qty;
  cash -= qty * price;
}
