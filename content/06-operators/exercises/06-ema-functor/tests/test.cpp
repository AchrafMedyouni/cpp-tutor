#include "doctest.h"
#include <algorithm>
#include <vector>
#include "ema.hpp"

TEST_CASE("update rule") {
  Ema e{ 0.5 };
  CHECK(e(10) == doctest::Approx(10));
  CHECK(e(20) == doctest::Approx(15));
  CHECK(e(20) == doctest::Approx(17.5));
  CHECK(e.value() == doctest::Approx(17.5));
}
TEST_CASE("alpha validation") {
  CHECK_THROWS_AS(Ema{ 0 }, std::invalid_argument);
  CHECK_THROWS_AS(Ema{ 1.5 }, std::invalid_argument);
}
TEST_CASE("works with std::transform") {
  std::vector<double> px{ 1, 2, 3 }, out(3);
  std::transform(px.begin(), px.end(), out.begin(), Ema{ 1.0 });   // alpha 1: follows the input
  CHECK(out == px);
}
