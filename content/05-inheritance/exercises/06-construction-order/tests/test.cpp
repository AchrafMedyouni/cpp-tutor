#include "doctest.h"
#include "instruments.hpp"

TEST_CASE("construction and destruction order") {
  trace().clear();
  {
    Bond b{ "OAT 2034", 0.035 };
    CHECK(b.name() == "OAT 2034");
    CHECK(b.coupon() == doctest::Approx(0.035));
  }
  std::vector<std::string> expected{ "Instrument(OAT 2034)", "Bond", "~Bond", "~Instrument" };
  CHECK(trace() == expected);
}
TEST_CASE("destruction through the base pointer") {
  trace().clear();
  Instrument* i = new Bond{ "BTP", 0.04 };
  delete i;
  REQUIRE(trace().size() == 4);
  CHECK(trace()[2] == "~Bond");
}
