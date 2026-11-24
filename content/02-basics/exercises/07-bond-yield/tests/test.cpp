#include "doctest.h"
#include <cmath>
#include "yield.hpp"

TEST_CASE("bond at par") { CHECK(bond_price(100, 0.05, 5, 0.05) == doctest::Approx(100)); }
TEST_CASE("bond below par when yield > coupon") {
  CHECK(bond_price(100, 0.05, 5, 0.06) == doctest::Approx(95.787636));
}
TEST_CASE("bisect finds sqrt(2)") {
  CHECK(bisect([](double x) { return x * x - 2; }, 0, 2) == doctest::Approx(std::sqrt(2.0)));
}
TEST_CASE("yield to maturity") {
  CHECK(yield_to_maturity(100, 100, 0.05, 5) == doctest::Approx(0.05));
  CHECK(yield_to_maturity(95.787636, 100, 0.05, 5) == doctest::Approx(0.06));
  CHECK(yield_to_maturity(108.53, 100, 0.07, 10) == doctest::Approx(0.0585).epsilon(1e-3));
}
