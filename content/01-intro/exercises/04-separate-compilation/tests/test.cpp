#include "doctest.h"
#include "returns.hpp"
#include "returns.hpp"   // twice on purpose: needs an include guard

TEST_CASE("trading_days constant") { CHECK(trading_days == 252); }

TEST_CASE("simple return") {
  CHECK(simple_return(100.0, 110.0) == doctest::Approx(0.10));
  CHECK(simple_return(50.0, 25.0) == doctest::Approx(-0.5));
}

TEST_CASE("log return") {
  CHECK(log_return(100.0, 100.0) == doctest::Approx(0.0));
  CHECK(log_return(1.0, 2.718281828459045) == doctest::Approx(1.0));
}

TEST_CASE("annualized volatility") {
  CHECK(annualized_vol(0.01) == doctest::Approx(0.158745).epsilon(1e-5));
}
