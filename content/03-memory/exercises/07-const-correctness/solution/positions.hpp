#pragma once

struct Position { long qty; double price; };

inline double total(const Position* ps, int n) {
  double t = 0;
  for (int i = 0; i < n; ++i) t += ps[i].qty * ps[i].price;
  return t;
}

inline void scale(Position& p, double factor) {
  p.qty = static_cast<long>(p.qty * factor);
}

inline double notional(const Position& p) { return p.qty * p.price; }

inline const Position* best(const Position* ps, int n) {
  const Position* b = ps;
  for (int i = 1; i < n; ++i)
    if (notional(ps[i]) > notional(*b)) b = &ps[i];
  return b;
}
