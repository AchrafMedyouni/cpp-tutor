#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include "Portfolio.hpp"

TEST_CASE("fills, lookup, valuation and automatic cleanup") {
  long before = cpplab::live_allocations();
  {
    Portfolio p{ 1 };
    p.add_fill("AAPL", 100, 180.0);
    p.add_fill("AAPL", 100, 190.0);
    p.add_fill("MSFT", 10, 400.0);
    p.add_fill("NVDA", -5, 900.0);
    CHECK(p.size() == 3);
    REQUIRE(p.find("AAPL") != nullptr);
    CHECK(p.find("AAPL")->avg_price == doctest::Approx(185.0));
    CHECK(p.find("TSLA") == nullptr);
    CHECK(p.market_value({ { "AAPL", 190.0 }, { "MSFT", 410.0 } }) == doctest::Approx(42100.0));
  }
  CHECK(cpplab::live_allocations() == before);
}
