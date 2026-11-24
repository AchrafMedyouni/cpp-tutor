#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include <type_traits>
#include "instruments.hpp"

TEST_CASE("Instrument is an abstract base with a virtual destructor") {
  CHECK(std::is_abstract_v<Instrument>);
  CHECK(std::has_virtual_destructor_v<Instrument>);
  CHECK(std::is_base_of_v<Instrument, Bond>);
}
TEST_CASE("bond pricing through a base reference") {
  Bond b{ "OAT 5Y", 100, 0.05, 5 };
  const Instrument& i = b;
  CHECK(i.name() == "OAT 5Y");
  CHECK(i.price(Market{ 0, 0.05, 0 }) == doctest::Approx(100));
  CHECK(i.price(Market{ 0, 0.06, 0 }) == doctest::Approx(95.787636));
}
