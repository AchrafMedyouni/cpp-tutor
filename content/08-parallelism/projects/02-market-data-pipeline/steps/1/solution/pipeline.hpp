#pragma once
#include <deque>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include "blocking_queue.hpp"

struct Tick {
  std::string symbol;
  double price;
  long size;
};

// "SYMBOL,price,size" → Tick; nullopt on any malformed line.
inline std::optional<Tick> parse_tick(const std::string& line) {
  std::istringstream in{ line };
  std::string symbol, price_s, size_s;
  if (!std::getline(in, symbol, ',') || !std::getline(in, price_s, ',') || !std::getline(in, size_s)) return std::nullopt;
  if (symbol.empty()) return std::nullopt;
  try {
    std::size_t used = 0;
    double price = std::stod(price_s, &used);
    if (used != price_s.size() || price <= 0) return std::nullopt;
    long size = std::stol(size_s, &used);
    if (used != size_s.size() || size <= 0) return std::nullopt;
    return Tick{ symbol, price, size };
  } catch (...) {
    return std::nullopt;
  }
}

struct PipelineStats {
  long received = 0;
  long rejected = 0;
};
template <typename OnTick>
PipelineStats run_pipeline(const std::vector<std::string>& lines, OnTick on_tick, std::size_t capacity = 64) {
  (void)lines; (void)on_tick; (void)capacity;
  return {};   // step 2
}

struct Signal {
  std::string symbol;
  bool buy;
  double price;
  bool operator==(const Signal&) const = default;
};
class SmaCrossStrategy {
public:
  SmaCrossStrategy(std::size_t fast, std::size_t slow) { (void)fast; (void)slow; }
  void on_tick(const Tick& t) { (void)t; }   // step 3
  const std::vector<Signal>& signals() const { return signals_; }
private:
  std::vector<Signal> signals_;
};
