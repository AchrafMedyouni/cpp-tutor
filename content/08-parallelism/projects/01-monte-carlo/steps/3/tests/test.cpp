#include "doctest.h"
#include <cmath>
#include "monte_carlo.hpp"

namespace {
const McParams P{ 100, 100, 0.05, 0.2, 1.0 };   // BS price 10.4506
}

TEST_CASE("mutex and atomic versions agree with Black-Scholes") {
  CHECK(std::fabs(mc_price_mutex(P, 100'000, 4, 99) - bs_call(P)) < 0.25);
  CHECK(std::fabs(mc_price_atomic(P, 100'000, 4, 99) - bs_call(P)) < 0.25);
}
TEST_CASE("all three versions use the same paths") {
  double a = mc_price_threads(P, 40'000, 4, 5);
  CHECK(mc_price_mutex(P, 40'000, 4, 5, 40'000) == doctest::Approx(a).epsilon(1e-12));
  CHECK(mc_price_atomic(P, 40'000, 4, 5, 40'000) == doctest::Approx(a).epsilon(1e-12));
}
