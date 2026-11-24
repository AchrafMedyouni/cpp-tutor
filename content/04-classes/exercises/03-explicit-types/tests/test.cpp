#include "doctest.h"
#include <type_traits>
#include "types.hpp"

TEST_CASE("no implicit conversions") {
  CHECK_FALSE(std::is_convertible_v<double, Price>);
  CHECK_FALSE(std::is_convertible_v<long, Quantity>);
  CHECK(std::is_constructible_v<Price, double>);
}
TEST_CASE("notional") { CHECK(notional(Price{ 12.5 }, Quantity{ 8 }) == doctest::Approx(100)); }
TEST_CASE("price validation") { CHECK_THROWS_AS(Price{ -1.0 }, std::invalid_argument); }
