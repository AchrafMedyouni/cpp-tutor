#include "money.hpp"

struct Portfolio {
  Money cash;
  long shares;
  Money last_price;
};

inline long value(Portfolio p) { return cents(p.cash) + p.shares * cents(p.last_price); }
