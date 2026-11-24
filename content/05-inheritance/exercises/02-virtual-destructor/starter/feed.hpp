#pragma once
#include <string>

class MarketFeed {
public:
  ~MarketFeed() {}
  virtual std::string name() const = 0;
  inline static int closed = 0;      // incremented by derived destructors
};

class EquityFeed : public MarketFeed {
public:
  explicit EquityFeed(int depth) : depth_{ depth }, book_{ new double[depth] } {}
  ~EquityFeed() { delete[] book_; ++closed; }
  std::string name() const override { return "equities"; }
  int depth() const { return depth_; }
private:
  int depth_;
  double* book_;
};

inline MarketFeed* make_feed() { return new EquityFeed{ 10 }; }
