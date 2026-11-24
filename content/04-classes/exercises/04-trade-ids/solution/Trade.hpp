#pragma once
#include <string>

class Trade {
public:
  Trade(std::string symbol, long qty) : symbol_{ std::move(symbol) }, qty_{ qty }, id_{ next_id_++ } { ++live_; }
  Trade(const Trade& t) : symbol_{ t.symbol_ }, qty_{ t.qty_ }, id_{ next_id_++ } { ++live_; }
  Trade& operator=(const Trade& t) {   // keeps its own id
    symbol_ = t.symbol_;
    qty_ = t.qty_;
    return *this;
  }
  ~Trade() { --live_; }

  long id() const { return id_; }
  const std::string& symbol() const { return symbol_; }
  long qty() const { return qty_; }
  static long live() { return live_; }
  static void reset_ids() { next_id_ = 1; }

private:
  std::string symbol_;
  long qty_;
  long id_;
  inline static long next_id_{ 1 };
  inline static long live_{ 0 };
};
