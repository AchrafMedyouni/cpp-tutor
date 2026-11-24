#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include "portfolio.hpp"

TEST_CASE("make_portfolio allocates, destroy releases") {
  long before = cpplab::live_allocations();
  Portfolio p = make_portfolio(4);
  CHECK(p.capacity == 4);
  CHECK(p.size == 0);
  CHECK(p.positions != nullptr);
  CHECK(cpplab::live_allocations() > before);
  destroy(p);
  CHECK(p.positions == nullptr);
  CHECK(p.capacity == 0);
  CHECK(cpplab::live_allocations() == before);
}
