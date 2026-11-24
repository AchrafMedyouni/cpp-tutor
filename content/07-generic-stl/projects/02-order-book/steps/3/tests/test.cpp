#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include "order_book.hpp"

TEST_CASE("cancel") {
  OrderBook ob;
  ob.add_limit(1, Side::Buy, 100, 10);
  ob.add_limit(2, Side::Buy, 100, 20);
  ob.add_limit(3, Side::Buy, 99, 5);
  CHECK(ob.cancel(1));
  CHECK_FALSE(ob.cancel(1));
  CHECK_FALSE(ob.cancel(42));
  CHECK(ob.volume_at(Side::Buy, 100) == 20);
  CHECK(ob.cancel(2));
  CHECK(*ob.best_bid() == 99);   // level 100 is gone
}
TEST_CASE("market orders never rest") {
  OrderBook ob;
  ob.add_limit(1, Side::Sell, 101, 10);
  ob.add_limit(2, Side::Sell, 103, 10);
  auto trades = ob.add_market(9, Side::Buy, 25);
  std::vector<Trade> expected{ { 9, 1, 101, 10 }, { 9, 2, 103, 10 } };
  CHECK(trades == expected);
  CHECK(ob.order_count() == 0);
  CHECK_FALSE(ob.best_bid().has_value());
  CHECK(ob.add_market(10, Side::Sell, 5).empty());
}
TEST_CASE("no leaks across adds, fills and cancels") {
  long before = cpplab::live_allocations();
  {
    OrderBook ob;
    for (long i = 0; i < 200; ++i) ob.add_limit(i, i % 2 ? Side::Buy : Side::Sell, 100 + (i % 7) - 3, 10);
    for (long i = 0; i < 200; i += 3) ob.cancel(i);
    ob.add_market(1000, Side::Buy, 300);
    ob.add_market(1001, Side::Sell, 300);
  }
  CHECK(cpplab::live_allocations() == before);
}
