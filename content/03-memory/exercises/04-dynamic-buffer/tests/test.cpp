#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include "buffer.hpp"

TEST_CASE("make, average, release without leak") {
  long before = cpplab::live_allocations();
  double* p = make_prices(5, 100, 0.5);
  REQUIRE(p != nullptr);
  CHECK(p[0] == 100);
  CHECK(p[4] == doctest::Approx(102));
  CHECK(average(p, 5) == doctest::Approx(101));
  CHECK(cpplab::live_allocations() == before + 1);
  release(p);
  CHECK(cpplab::live_allocations() == before);
}
TEST_CASE("grow copies and frees the old block") {
  long before = cpplab::live_allocations();
  double* p = make_prices(3, 1, 1);
  REQUIRE(p != nullptr);
  p = grow(p, 3, 6);
  CHECK(cpplab::live_allocations() == before + 1);   // the old block is gone
  CHECK(p[2] == 3);
  CHECK(p[5] == 0);
  release(p);
  CHECK(cpplab::live_allocations() == before);
}
