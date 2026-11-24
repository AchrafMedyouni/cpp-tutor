#include "doctest.h"
#include "desk.hpp"

TEST_CASE("each vendor lives in its own namespace") {
  CHECK(vendor_a::bid(100.0) == doctest::Approx(99.98));
  CHECK(vendor_b::bid(100.0) == doctest::Approx(99.99));
  CHECK(vendor_a::name() == "Vendor A");
  CHECK(vendor_b::name() == "Vendor B");
}

TEST_CASE("desk::best_bid picks the higher bid") {
  CHECK(desk::best_bid(100.0) == doctest::Approx(99.99));   // B wins at 100
  CHECK(desk::best_bid(500.0) == doctest::Approx(499.98));  // A wins at 500
}

TEST_CASE("alias va") { CHECK(va::name() == "Vendor A"); }
