#pragma once
#include <map>
#include <string>
#include <utility>

struct Position {
  std::string symbol;
  long qty = 0;
  double avg_price = 0;
};

class Portfolio {
public:
  explicit Portfolio(int capacity = 4) : size_{ 0 }, capacity_{ capacity > 0 ? capacity : 1 }, positions_{ new Position[capacity_] } {}
  ~Portfolio() { delete[] positions_; }

  Portfolio(const Portfolio&) = delete;              // step 2
  Portfolio& operator=(const Portfolio&) = delete;

  int size() const { return size_; }

  const Position* find(const std::string& symbol) const {
    for (int i = 0; i < size_; ++i)
      if (positions_[i].symbol == symbol) return &positions_[i];
    return nullptr;
  }

  void add_fill(const std::string& symbol, long qty, double price) {
    Position* pos = const_cast<Position*>(find(symbol));
    if (!pos) {
      if (size_ == capacity_) grow();
      pos = &positions_[size_++];
      pos->symbol = symbol;
    }
    long new_qty = pos->qty + qty;
    if (qty > 0 && new_qty != 0) pos->avg_price = (pos->qty * pos->avg_price + qty * price) / new_qty;
    pos->qty = new_qty;
  }

  double market_value(const std::map<std::string, double>& marks) const {
    double v = 0;
    for (int i = 0; i < size_; ++i) {
      auto it = marks.find(positions_[i].symbol);
      if (it != marks.end()) v += positions_[i].qty * it->second;
    }
    return v;
  }

private:
  void grow() {
    Position* bigger = new Position[2 * capacity_];
    for (int i = 0; i < size_; ++i) bigger[i] = positions_[i];
    delete[] positions_;
    positions_ = bigger;
    capacity_ *= 2;
  }
  int size_;
  int capacity_;
  Position* positions_;
};
