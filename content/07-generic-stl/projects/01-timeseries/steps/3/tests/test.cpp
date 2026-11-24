#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include <cmath>
#include "timeseries.hpp"

TEST_CASE("drawdown") {
  CHECK(max_drawdown(TimeSeries<double>{ 100, 120, 90, 130, 117 }) == doctest::Approx(0.25));
  CHECK(max_drawdown(TimeSeries<double>{ 1, 2, 3 }) == doctest::Approx(0));
  CHECK(max_drawdown(TimeSeries<double>{}) == 0);
}
TEST_CASE("annualised volatility") {
  TimeSeries<double> r{ 0.01, -0.01, 0.01, -0.01 };
  CHECK(annualized_volatility(r) == doctest::Approx(std::sqrt(4.0 / 3 * 0.0001) * std::sqrt(252.0)));
  CHECK(annualized_volatility(r, 12) == doctest::Approx(std::sqrt(4.0 / 3 * 0.0001) * std::sqrt(12.0)));
  CHECK(annualized_volatility(TimeSeries<double>{ 0.5 }) == 0);
}
