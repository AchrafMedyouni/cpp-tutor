#include "doctest.h"
#include "Trade.hpp"

TEST_CASE("ids increase") {
  Trade::reset_ids();
  Trade a{ "AAPL", 10 }, b{ "MSFT", 5 };
  CHECK(a.id() == 1);
  CHECK(b.id() == 2);
}
TEST_CASE("a copy gets a new id") {
  Trade::reset_ids();
  Trade a{ "AAPL", 10 };
  Trade c{ a };
  CHECK(c.symbol() == "AAPL");
  CHECK(c.qty() == 10);
  CHECK(c.id() == 2);
}
TEST_CASE("live count") {
  long base = Trade::live();
  {
    Trade a{ "X", 1 };
    Trade b{ a };
    CHECK(Trade::live() == base + 2);
  }
  CHECK(Trade::live() == base);
}
