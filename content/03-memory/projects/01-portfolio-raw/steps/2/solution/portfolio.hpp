#pragma once
#include <string>

struct Position {
  std::string symbol;
  long qty = 0;
  double avg_price = 0;   // average cost of the current position
};

struct Portfolio {
  Position* positions = nullptr;   // array allocated with new[]
  int size = 0;                    // positions in use
  int capacity = 0;                // allocated length
};

inline Portfolio make_portfolio(int capacity) {
  Portfolio p;
  p.positions = new Position[capacity];
  p.capacity = capacity;
  return p;
}

inline void destroy(Portfolio& p) {
  delete[] p.positions;
  p.positions = nullptr;
  p.size = p.capacity = 0;
}

inline Position* find(Portfolio& p, const std::string& symbol) {
  for (Position* it = p.positions; it != p.positions + p.size; ++it)
    if (it->symbol == symbol) return it;
  return nullptr;
}

inline void grow(Portfolio& p) {
  int new_cap = p.capacity > 0 ? 2 * p.capacity : 1;
  Position* bigger = new Position[new_cap];
  for (int i = 0; i < p.size; ++i) bigger[i] = p.positions[i];
  delete[] p.positions;
  p.positions = bigger;
  p.capacity = new_cap;
}

inline void add_fill(Portfolio& p, const std::string& symbol, long qty, double price) {
  Position* pos = find(p, symbol);
  if (!pos) {
    if (p.size == p.capacity) grow(p);
    pos = &p.positions[p.size++];
    pos->symbol = symbol;
  }
  long new_qty = pos->qty + qty;
  if (qty > 0 && new_qty != 0)   // buying: update the average cost
    pos->avg_price = (pos->qty * pos->avg_price + qty * price) / new_qty;
  pos->qty = new_qty;
}

// Step 3: not done yet
inline double market_value(const Portfolio& p, const std::string* symbols, const double* marks, int n) {
  (void)p; (void)symbols; (void)marks; (void)n;
  return 0;
}
inline double unrealized_pnl(Portfolio& p, const std::string& symbol, double mark) { (void)p; (void)symbol; (void)mark; return 0; }
