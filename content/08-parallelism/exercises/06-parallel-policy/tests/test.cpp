#include "doctest.h"
#include "par.hpp"

TEST_CASE("sum of squares") {
  std::vector<double> v(100000, 2.0);
  CHECK(sum_of_squares(v) == doctest::Approx(400000));
  CHECK(sum_of_squares({}) == 0);
}
TEST_CASE("normalize") {
  std::vector<double> v{ 1, 2, 3, 4, 5 };
  normalize(v);
  CHECK(v[0] == doctest::Approx(-2));
  CHECK(v[4] == doctest::Approx(2));
}
TEST_CASE("outliers") {
  std::vector<double> v{ 0.5, -3, 2.5, 0, -0.1, 4 };
  CHECK(count_outliers(v, 1.0) == 3);
}
