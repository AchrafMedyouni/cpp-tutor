#include "doctest.h"
#include "analytics.hpp"

TEST_CASE("returns and mean") {
  auto r = simple_returns({ 100, 110, 99 });
  REQUIRE(r.size() == 2);
  CHECK(r[0] == doctest::Approx(0.10));
  CHECK(r[1] == doctest::Approx(-0.10));
  CHECK(simple_returns({ 1 }).empty());
  CHECK(mean({ 1, 2, 3, 4 }) == doctest::Approx(2.5));
  CHECK(mean({}) == 0);
}
TEST_CASE("top_n") {
  CHECK(top_n({ 3, 9, 1, 7, 5 }, 3) == std::vector<double>{ 9, 7, 5 });
  CHECK(top_n({ 2, 1 }, 5) == std::vector<double>{ 2, 1 });
}
TEST_CASE("up days and drawdown") {
  std::vector<double> p{ 100, 120, 90, 130, 117, 118 };
  CHECK(count_up_days(p) == 3);
  CHECK(max_drawdown(p) == doctest::Approx(0.25));
  CHECK(max_drawdown({ 1, 2, 3 }) == doctest::Approx(0));
}
