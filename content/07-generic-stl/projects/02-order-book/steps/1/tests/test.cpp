#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include "order_book.hpp"

TEST_CASE("best prices and volumes") {
  long before = cpplab::live_allocations();
  {
    OrderBook ob;
    CHECK_FALSE(ob.best_bid().has_value());
    ob.add_limit(1, Side::Buy, 9950, 100);
    ob.add_limit(2, Side::Buy, 9975, 50);
    ob.add_limit(3, Side::Sell, 10025, 70);
    ob.add_limit(4, Side::Sell, 10010, 20);
    ob.add_limit(5, Side::Buy, 9975, 30);
    CHECK(*ob.best_bid() == 9975);
    CHECK(*ob.best_ask() == 10010);
    CHECK(ob.volume_at(Side::Buy, 9975) == 80);
    CHECK(ob.volume_at(Side::Sell, 9975) == 0);
    CHECK(ob.order_count() == 5);
  }
  CHECK(cpplab::live_allocations() == before);
}
