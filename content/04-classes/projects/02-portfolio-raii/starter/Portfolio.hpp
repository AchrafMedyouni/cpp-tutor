#pragma once
#include <map>
#include <string>
#include <utility>

struct Position {
  std::string symbol;
  long qty = 0;
  double avg_price = 0;
};

// TODO: turn chapter 3's struct + free functions into a class that manages its own memory.
class Portfolio {
public:
  explicit Portfolio(int capacity = 4) { (void)capacity; }
  int size() const { return 0; }
  const Position* find(const std::string& symbol) const { (void)symbol; return nullptr; }
  void add_fill(const std::string& symbol, long qty, double price) { (void)symbol; (void)qty; (void)price; }
  double market_value(const std::map<std::string, double>& marks) const { (void)marks; return 0; }
};
