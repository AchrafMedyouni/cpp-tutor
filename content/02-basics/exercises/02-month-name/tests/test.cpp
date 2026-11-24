#include "doctest.h"
#include "months.hpp"

TEST_CASE("names") {
  CHECK(month_name(1) == "January");
  CHECK(month_name(7) == "July");
  CHECK(month_name(12) == "December");
}
TEST_CASE("invalid month") {
  CHECK(month_name(0) == "unknown");
  CHECK(month_name(13) == "unknown");
}
TEST_CASE("days") {
  CHECK(days_in_month(1, false) == 31);
  CHECK(days_in_month(4, false) == 30);
  CHECK(days_in_month(2, false) == 28);
  CHECK(days_in_month(2, true) == 29);
  CHECK(days_in_month(11, true) == 30);
  CHECK(days_in_month(42, false) == 0);
}
