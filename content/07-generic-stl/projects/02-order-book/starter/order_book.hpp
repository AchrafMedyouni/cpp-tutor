#pragma once
#include <algorithm>
#include <cstddef>
#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

enum class Side { Buy, Sell };

struct Order {
  long id;
  Side side;
  long price;      // in ticks (integer: never compare doubles for equality)
  long qty;        // remaining quantity
};

struct Trade {
  long buy_id;
  long sell_id;
  long price;
  long qty;
  bool operator==(const Trade&) const = default;
};

class OrderBook {
public:
  // Step 1
  std::vector<Trade> add_limit(long id, Side side, long price, long qty) { (void)id; (void)side; (void)price; (void)qty; return {}; }
  std::optional<long> best_bid() const { return std::nullopt; }
  std::optional<long> best_ask() const { return std::nullopt; }
  long volume_at(Side side, long price) const { (void)side; (void)price; return 0; }
  std::size_t order_count() const { return 0; }
  // Step 3
  bool cancel(long id) { (void)id; return false; }
  std::vector<Trade> add_market(long id, Side side, long qty) { (void)id; (void)side; (void)qty; return {}; }
};
