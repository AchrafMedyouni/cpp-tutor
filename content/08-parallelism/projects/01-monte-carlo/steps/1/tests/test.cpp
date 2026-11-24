#include "doctest.h"
#include <cmath>
#include "monte_carlo.hpp"

namespace {
const McParams P{ 100, 100, 0.05, 0.2, 1.0 };   // BS price 10.4506
}

TEST_CASE("sequential price converges to Black-Scholes") {
  double mc = mc_price_seq(P, 100'000, 42);
  CHECK(std::fabs(mc - bs_call(P)) < 0.25);
  CHECK(mc_price_seq(P, 1000, 7) == mc_price_seq(P, 1000, 7));   // reproducible
}
