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

// Step 1
inline std::optional<Tick> parse_tick(const std::string& line) { (void)line; return std::nullopt; }   // TODO

// Step 2
struct PipelineStats {
  long received = 0;
  long rejected = 0;
};
template <typename OnTick>
PipelineStats run_pipeline(const std::vector<std::string>& lines, OnTick on_tick, std::size_t capacity = 64) {   // TODO
  (void)lines; (void)on_tick; (void)capacity;
  return {};
}

// Step 3
struct Signal {
  std::string symbol;
  bool buy;
  double price;
  bool operator==(const Signal&) const = default;
};
class SmaCrossStrategy {
public:
  SmaCrossStrategy(std::size_t fast, std::size_t slow) { (void)fast; (void)slow; }
  void on_tick(const Tick& t) { (void)t; }   // TODO
  const std::vector<Signal>& signals() const { return signals_; }
private:
  std::vector<Signal> signals_;
};
