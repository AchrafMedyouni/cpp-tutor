#include "doctest.h"
#include <string>
#include "side.hpp"

TEST_CASE("to_string") {
  CHECK(std::string(to_string(Side::Buy)) == "BUY");
  CHECK(std::string(to_string(Side::Sell)) == "SELL");
}
TEST_CASE("opposite and sign") {
  CHECK(opposite(Side::Buy) == Side::Sell);
  CHECK(opposite(Side::Sell) == Side::Buy);
  CHECK(sign(Side::Buy) == 1);
  CHECK(sign(Side::Sell) == -1);
}
TEST_CASE("parse") {
  CHECK(parse_side("BUY") == Side::Buy);
  CHECK(parse_side("S") == Side::Sell);
  CHECK_THROWS_AS(parse_side("HOLD"), std::invalid_argument);
  CHECK_THROWS_AS(parse_side(""), std::invalid_argument);
}
