#include "doctest.h"
#include <string>
#include "generic.hpp"

TEST_CASE("max_of") {
  CHECK(max_of(std::vector<int>{ 3, 9, 2 }) == 9);
  CHECK(max_of(std::vector<double>{ -1.5, -0.5 }) == -0.5);
  CHECK(max_of(std::vector<std::string>{ "AAPL", "TSLA", "MSFT" }) == "TSLA");
  CHECK_THROWS_AS(max_of(std::vector<int>{}), std::invalid_argument);
}
TEST_CASE("clamp_to") {
  CHECK(clamp_to(15, 0, 10) == 10);
  CHECK(clamp_to(-0.2, 0.0, 1.0) == 0.0);
  CHECK(clamp_to(0.4, 0.0, 1.0) == doctest::Approx(0.4));
}
TEST_CASE("count_equal") {
  CHECK(count_equal(std::vector<int>{ 1, 2, 1, 1 }, 1) == 3);
  CHECK(count_equal(std::vector<std::string>{ "B", "S", "B" }, std::string{ "S" }) == 1);
}
