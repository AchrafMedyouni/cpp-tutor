#include "doctest.h"
#include <cmath>
#include "monte_carlo.hpp"

namespace {
const McParams P{ 100, 100, 0.05, 0.2, 1.0 };   // BS price 10.4506
}

TEST_CASE("threaded price: accurate, reproducible, race-free") {
  for (unsigned k : { 1u, 3u, 4u }) {
    double mc = mc_price_threads(P, 100'000, k, 2024);
    CHECK(std::fabs(mc - bs_call(P)) < 0.25);
  }
  CHECK(mc_price_threads(P, 20'000, 4, 1) == mc_price_threads(P, 20'000, 4, 1));
  CHECK(mc_price_threads(P, 10, 16, 1) > 0);   // more threads than paths per thread
}
