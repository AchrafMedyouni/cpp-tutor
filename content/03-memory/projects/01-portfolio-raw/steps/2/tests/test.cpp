#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include <string>
#include "portfolio.hpp"

TEST_CASE("fills update quantity and average cost") {
  Portfolio p = make_portfolio(2);
  add_fill(p, "AAPL", 100, 180.0);
  add_fill(p, "AAPL", 100, 190.0);
  Position* a = find(p, "AAPL");
  REQUIRE(a != nullptr);
  CHECK(a->qty == 200);
  CHECK(a->avg_price == doctest::Approx(185.0));
  add_fill(p, "AAPL", -50, 200.0);   // a sell keeps the average cost
  CHECK(a->qty == 150);
  CHECK(a->avg_price == doctest::Approx(185.0));
  CHECK(find(p, "MSFT") == nullptr);
  destroy(p);
}

TEST_CASE("growth beyond the initial capacity, without leaks") {
  long before = cpplab::live_allocations();
  Portfolio p = make_portfolio(1);
  const char* syms[] = { "A", "B", "C", "D", "E", "F", "G" };
  for (int i = 0; i < 7; ++i) add_fill(p, syms[i], i + 1, 10.0);
  CHECK(p.size == 7);
  CHECK(p.capacity >= 7);
  REQUIRE(find(p, "G") != nullptr);
  CHECK(find(p, "G")->qty == 7);
  CHECK(find(p, "A")->qty == 1);
  destroy(p);
  CHECK(cpplab::live_allocations() == before);
}
