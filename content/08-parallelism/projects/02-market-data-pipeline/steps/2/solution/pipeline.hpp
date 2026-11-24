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
  long received = 0;   // ticks delivered to the consumer
  long rejected = 0;   // malformed lines dropped by the producer
};

// Producer thread parses and pushes; the calling thread consumes and calls on_tick for every tick, in order.
template <typename OnTick>
PipelineStats run_pipeline(const std::vector<std::string>& lines, OnTick on_tick, std::size_t capacity = 64) {
  BlockingQueue<Tick> queue{ capacity };
  PipelineStats stats;
  long rejected = 0;
  std::thread producer{ [&] {
    for (const std::string& line : lines) {
      if (auto t = parse_tick(line)) queue.push(std::move(*t));
      else ++rejected;                 // only the producer touches `rejected` before join
    }
    queue.close();
  } };
  while (auto t = queue.pop()) {
    on_tick(*t);
    ++stats.received;
  }
  producer.join();                     // after join, reading `rejected` is safe
  stats.rejected = rejected;
  return stats;
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
