#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include <cmath>
#include "instruments.hpp"

TEST_CASE("Black-Scholes reference values") {
  Market m{ 100, 0.05, 0.2 };
  EuropeanOption call{ "C", true, 100, 1.0 }, put{ "P", false, 100, 1.0 };
  CHECK(call.price(m) == doctest::Approx(10.450584).epsilon(1e-6));
  CHECK(put.price(m) == doctest::Approx(5.573526).epsilon(1e-6));
}
TEST_CASE("put-call parity") {
  Market m{ 87, 0.03, 0.35 };
  EuropeanOption c{ "C", true, 95, 0.5 }, p{ "P", false, 95, 0.5 };
  CHECK(c.price(m) - p.price(m) == doctest::Approx(87 - 95 * std::exp(-0.03 * 0.5)));
}
