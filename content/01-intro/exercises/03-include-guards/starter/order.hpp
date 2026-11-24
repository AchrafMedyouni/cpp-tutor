#include "money.hpp"

struct Order {
  Money price;
  long quantity;
};

inline long notional(Order o) { return cents(o.price) * o.quantity; }
