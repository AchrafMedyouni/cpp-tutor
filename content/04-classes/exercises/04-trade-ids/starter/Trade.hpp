#pragma once
#include <string>

class Trade {
public:
  Trade(std::string symbol, long qty) : symbol_{ std::move(symbol) }, qty_{ qty }, id_{ 0 } {}
  long id() const { return id_; }
  const std::string& symbol() const { return symbol_; }
  long qty() const { return qty_; }
  static long live() { return 0; }   // TODO
  static void reset_ids() {}         // TODO
private:
  std::string symbol_;
  long qty_;
  long id_;
};
