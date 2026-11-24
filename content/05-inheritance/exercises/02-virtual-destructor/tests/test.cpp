#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include "feed.hpp"

TEST_CASE("deleting through the base pointer runs the derived destructor") {
  long before = cpplab::live_allocations();
  int closed_before = MarketFeed::closed;
  MarketFeed* f = make_feed();
  CHECK(f->name() == "equities");
  delete f;
  CHECK(MarketFeed::closed == closed_before + 1);
  CHECK(cpplab::live_allocations() == before);
}
