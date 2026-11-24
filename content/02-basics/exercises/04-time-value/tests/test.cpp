#include "doctest.h"
#include "tvm.hpp"

TEST_CASE("future value, annual by default") {
  CHECK(future_value(1000, 0.05, 10) == doctest::Approx(1628.894627));
}
TEST_CASE("future value, monthly") {
  CHECK(future_value(1000, 0.05, 10, 12) == doctest::Approx(1647.009498));
}
TEST_CASE("present value is the inverse") {
  CHECK(present_value(1628.894627, 0.05, 10) == doctest::Approx(1000));
  CHECK(present_value(future_value(250, 0.03, 7, 4), 0.03, 7, 4) == doctest::Approx(250));
}
TEST_CASE("annuity payment") {
  CHECK(annuity_payment(200000, 0.04 / 12, 360) == doctest::Approx(954.83).epsilon(1e-4));   // 30y mortgage at 4%
  CHECK(annuity_payment(1200, 0.0, 12) == doctest::Approx(100));
}
