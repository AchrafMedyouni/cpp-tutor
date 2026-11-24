#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include "Portfolio.hpp"

TEST_CASE("snapshots are independent") {
  long before = cpplab::live_allocations();
  {
    Portfolio live{ 2 };
    live.add_fill("AAPL", 100, 180.0);
    Portfolio snapshot{ live };
    snapshot.add_fill("AAPL", 100, 200.0);
    snapshot.add_fill("MSFT", 1, 400.0);
    CHECK(live.find("AAPL")->qty == 100);
    CHECK(live.size() == 1);
    CHECK(snapshot.find("AAPL")->qty == 200);

    Portfolio other;
    other = snapshot;
    other.add_fill("GOOG", 3, 150.0);
    CHECK(snapshot.size() == 2);
    CHECK(other.size() == 3);
    Portfolio& same = other;
    other = same;
    CHECK(other.size() == 3);
  }
  CHECK(cpplab::live_allocations() == before);
}
