#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include "order_book.hpp"

TEST_CASE("crossing buy sweeps the asks with price-time priority") {
  OrderBook ob;
  ob.add_limit(1, Side::Sell, 101, 50);
  ob.add_limit(2, Side::Sell, 100, 30);
  ob.add_limit(3, Side::Sell, 100, 40);   // same level, later than #2
  auto trades = ob.add_limit(10, Side::Buy, 101, 100);
  std::vector<Trade> expected{ { 10, 2, 100, 30 }, { 10, 3, 100, 40 }, { 10, 1, 101, 30 } };
  CHECK(trades == expected);
  CHECK(*ob.best_ask() == 101);
  CHECK(ob.volume_at(Side::Sell, 101) == 20);
  CHECK_FALSE(ob.best_bid().has_value());
}
TEST_CASE("remainder rests, and sells hit the highest bid") {
  OrderBook ob;
  ob.add_limit(1, Side::Buy, 99, 10);
  ob.add_limit(2, Side::Buy, 98, 10);
  auto trades = ob.add_limit(3, Side::Sell, 98, 25);
  std::vector<Trade> expected{ { 1, 3, 99, 10 }, { 2, 3, 98, 10 } };
  CHECK(trades == expected);
  CHECK(*ob.best_ask() == 98);
  CHECK(ob.volume_at(Side::Sell, 98) == 5);
  CHECK(ob.order_count() == 1);
}
TEST_CASE("no cross, no trade") {
  OrderBook ob;
  ob.add_limit(1, Side::Sell, 105, 10);
  CHECK(ob.add_limit(2, Side::Buy, 104, 10).empty());
  CHECK(ob.order_count() == 2);
}
