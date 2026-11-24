#pragma once

struct Position { long qty; double price; };

inline double total(Position* ps, int n) {           // only reads
  double t = 0;
  for (int i = 0; i < n; ++i) t += ps[i].qty * ps[i].price;
  return t;
}

inline void scale(Position p, double factor) {       // should modify the caller's position
  p.qty = static_cast<long>(p.qty * factor);
}

inline double notional(Position& p) { return p.qty * p.price; }   // called with temporaries

inline Position* best(Position* ps, int n) {          // largest notional
  Position* b = ps;
  for (int i = 1; i < n; ++i)
    if (notional(ps[i]) > notional(*b)) b = &ps[i];
  return b;
}
