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

// Step 1
inline Portfolio make_portfolio(int capacity) { (void)capacity; return Portfolio{}; }   // TODO
inline void destroy(Portfolio& p) { (void)p; }                                         // TODO

// Step 2
inline Position* find(Portfolio& p, const std::string& symbol) { (void)p; (void)symbol; return nullptr; }   // TODO
inline void add_fill(Portfolio& p, const std::string& symbol, long qty, double price) {                     // TODO
  (void)p; (void)symbol; (void)qty; (void)price;
}

// Step 3
inline double market_value(const Portfolio& p, const std::string* symbols, const double* marks, int n) {   // TODO
  (void)p; (void)symbols; (void)marks; (void)n;
  return 0;
}
inline double unrealized_pnl(Portfolio& p, const std::string& symbol, double mark) { (void)p; (void)symbol; (void)mark; return 0; }   // TODO
