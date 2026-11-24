#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include "PriceRing.hpp"

TEST_CASE("fills up then overwrites the oldest") {
  PriceRing r{ 3 };
  CHECK(r.size() == 0);
  CHECK(r.mean() == 0);
  r.push(1); r.push(2);
  CHECK(r.size() == 2);
  CHECK(r.at(0) == 1);
  r.push(3); r.push(4);
  CHECK(r.size() == 3);
  CHECK(r.at(0) == 2);
  CHECK(r.at(2) == 4);
  CHECK(r.mean() == doctest::Approx(3));
  CHECK_THROWS_AS(r.at(3), std::out_of_range);
}
TEST_CASE("zero capacity") { CHECK_THROWS_AS(PriceRing{ 0 }, std::invalid_argument); }
TEST_CASE("copies are independent and nothing leaks") {
  long before = cpplab::live_allocations();
  {
    PriceRing a{ 2 };
    a.push(10); a.push(20);
    PriceRing b{ a };
    b.push(30);
    CHECK(a.at(0) == 10);
    CHECK(b.at(0) == 20);
    PriceRing c{ 5 };
    c = b;
    CHECK(c.capacity() == 2);
    CHECK(c.at(1) == 30);
  }
  CHECK(cpplab::live_allocations() == before);
}
