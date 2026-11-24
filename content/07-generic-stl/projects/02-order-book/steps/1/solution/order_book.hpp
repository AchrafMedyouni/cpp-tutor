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
  OrderBook() = default;
  OrderBook(const OrderBook&) = delete;
  OrderBook& operator=(const OrderBook&) = delete;

  std::optional<long> best_bid() const { return bids_.empty() ? std::nullopt : std::optional<long>{ bids_.begin()->first }; }
  std::optional<long> best_ask() const { return asks_.empty() ? std::nullopt : std::optional<long>{ asks_.begin()->first }; }

  long volume_at(Side side, long price) const {
    long v = 0;
    auto sum = [&](const auto& book) {
      auto it = book.find(price);
      if (it != book.end()) for (const Order* o : it->second) v += o->qty;
    };
    if (side == Side::Buy) sum(bids_); else sum(asks_);
    return v;
  }

  std::size_t order_count() const { return orders_.size(); }

  std::vector<Trade> add_limit(long id, Side side, long price, long qty) {
    rest(id, side, price, qty);
    return {};
  }

  bool cancel(long id) { (void)id; return false; }
  std::vector<Trade> add_market(long id, Side side, long qty) { (void)id; (void)side; (void)qty; return {}; }

private:
  void rest(long id, Side side, long price, long qty) {
    auto owned = std::make_unique<Order>(Order{ id, side, price, qty });
    Order* raw = owned.get();
    orders_.emplace(id, std::move(owned));     // the book owns every resting order
    if (side == Side::Buy) bids_[price].push_back(raw);
    else asks_[price].push_back(raw);
  }

  std::unordered_map<long, std::unique_ptr<Order>> orders_;               // owner
  std::map<long, std::deque<Order*>, std::greater<long>> bids_;          // best (highest) first, FIFO per level
  std::map<long, std::deque<Order*>> asks_;                              // best (lowest) first
};
