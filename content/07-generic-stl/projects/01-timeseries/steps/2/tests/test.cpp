#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include "timeseries.hpp"

TEST_CASE("returns") {
  auto r = simple_returns(TimeSeries<double>{ 100, 110, 99 });
  REQUIRE(r.size() == 2);
  CHECK(r[0] == doctest::Approx(0.10));
  CHECK(r[1] == doctest::Approx(-0.10));
  CHECK(simple_returns(TimeSeries<double>{ 5 }).empty());
}
TEST_CASE("moving average") {
  auto ma = moving_average(TimeSeries<double>{ 1, 2, 3, 4, 5, 6 }, 3);
  REQUIRE(ma.size() == 4);
  CHECK(ma[0] == doctest::Approx(2));
  CHECK(ma[3] == doctest::Approx(5));
  CHECK(moving_average(TimeSeries<double>{ 1, 2 }, 3).empty());
  CHECK_THROWS_AS(moving_average(TimeSeries<double>{ 1 }, 0), std::invalid_argument);
}
