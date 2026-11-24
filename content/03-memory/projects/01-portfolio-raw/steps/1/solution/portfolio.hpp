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

// Steps 2 and 3: not done yet
inline Position* find(Portfolio& p, const std::string& symbol) { (void)p; (void)symbol; return nullptr; }
inline void add_fill(Portfolio& p, const std::string& symbol, long qty, double price) {
  (void)p; (void)symbol; (void)qty; (void)price;
}
inline double market_value(const Portfolio& p, const std::string* symbols, const double* marks, int n) {
  (void)p; (void)symbols; (void)marks; (void)n;
  return 0;
}
inline double unrealized_pnl(Portfolio& p, const std::string& symbol, double mark) { (void)p; (void)symbol; (void)mark; return 0; }
