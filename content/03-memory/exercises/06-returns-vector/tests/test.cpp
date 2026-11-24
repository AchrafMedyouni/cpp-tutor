#include "doctest.h"
#include <cmath>
#include "stats.hpp"

TEST_CASE("log returns") {
  auto r = log_returns({ 100, 110, 99 });
  REQUIRE(r.size() == 2);
  CHECK(r[0] == doctest::Approx(std::log(1.1)));
  CHECK(r[1] == doctest::Approx(std::log(0.9)));
  CHECK(log_returns({ 100 }).empty());
}
TEST_CASE("sample stdev") {
  CHECK(sample_stdev({ 2, 4, 4, 4, 5, 5, 7, 9 }) == doctest::Approx(2.13809));
  CHECK(sample_stdev({ 1 }) == 0);
}
TEST_CASE("max drawdown") {
  CHECK(max_drawdown({ 100, 120, 90, 130, 117 }) == doctest::Approx(0.25));
  CHECK(max_drawdown({ 1, 2, 3 }) == 0);
}
