#include "doctest.h"
#include "pipeline.hpp"

TEST_CASE("valid lines") {
  auto t = parse_tick("AAPL,189.52,300");
  REQUIRE(t.has_value());
  CHECK(t->symbol == "AAPL");
  CHECK(t->price == doctest::Approx(189.52));
  CHECK(t->size == 300);
}
TEST_CASE("malformed lines") {
  for (const char* bad : { "", "AAPL", "AAPL,1.0", ",1.0,5", "AAPL,abc,5", "AAPL,12.5x,5", "AAPL,-1,5", "AAPL,1.0,0", "AAPL,1.0,5.5" })
    CHECK_FALSE(parse_tick(bad).has_value());
}
