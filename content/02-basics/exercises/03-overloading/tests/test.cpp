#include "doctest.h"
#include <string>
#include "max3.hpp"

TEST_CASE("int") {
  CHECK(max3(1, 5, 3) == 5);
  CHECK(max3(-1, -5, -3) == -1);
  CHECK(max3(7, 7, 2) == 7);
}
TEST_CASE("double") {
  CHECK(max3(1.5, 0.25, -3.0) == 1.5);
  CHECK(max3(0.1, 0.2, 0.3) == 0.3);
}
TEST_CASE("string") {
  CHECK(max3(std::string("AAPL"), std::string("MSFT"), std::string("GOOG")) == "MSFT");
}
TEST_CASE("two ints") { CHECK(max3(4, 9) == 9); }
