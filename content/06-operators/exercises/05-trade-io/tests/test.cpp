#include "doctest.h"
#include <sstream>
#include <vector>
#include "trade_io.hpp"

TEST_CASE("printing") {
  std::ostringstream os;
  os << Trade{ "AAPL", true, 100, 189.5 } << " | " << 1.0 / 3;
  CHECK(os.str() == "AAPL BUY 100 @ 189.50 | 0.333333");   // no sticky fixed mode
}
TEST_CASE("round trip") {
  std::istringstream in{ "MSFT SELL 20 @ 410.25\nNVDA BUY 5 @ 950" };
  std::vector<Trade> v;
  Trade t{};
  while (in >> t) v.push_back(t);
  REQUIRE(v.size() == 2);
  CHECK(v[0].symbol == "MSFT");
  CHECK_FALSE(v[0].buy);
  CHECK(v[0].qty == 20);
  CHECK(v[1].price == doctest::Approx(950));
}
TEST_CASE("errors set failbit and leave the trade unchanged") {
  Trade t{ "KEEP", true, 1, 1 };
  std::istringstream bad_side{ "AAPL HOLD 10 @ 1" };
  CHECK_FALSE(static_cast<bool>(bad_side >> t));
  CHECK(t.symbol == "KEEP");
  std::istringstream bad_at{ "AAPL BUY 10 at 1" };
  CHECK_FALSE(static_cast<bool>(bad_at >> t));
  std::istringstream bad_qty{ "AAPL BUY ten @ 1" };
  CHECK_FALSE(static_cast<bool>(bad_qty >> t));
  CHECK(t.qty == 1);
}
