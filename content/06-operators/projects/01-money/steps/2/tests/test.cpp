#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include <sstream>
#include "Money.hpp"

TEST_CASE("printing") {
  std::ostringstream os;
  os << Money{ 1234, "EUR" } << " | " << Money{ -5, "USD" } << " | " << Money{ 700, "JPY" };
  CHECK(os.str() == "12.34 EUR | -0.05 USD | 7.00 JPY");
}
TEST_CASE("parsing") {
  std::istringstream in{ "12.34 EUR 7 JPY 0.5 EUR -1.05 USD" };
  Money m{ 0, "XXX" };
  CHECK(static_cast<bool>(in >> m)); CHECK(m == Money{ 1234, "EUR" });
  CHECK(static_cast<bool>(in >> m)); CHECK(m == Money{ 700, "JPY" });
  CHECK(static_cast<bool>(in >> m)); CHECK(m == Money{ 50, "EUR" });
  CHECK(static_cast<bool>(in >> m)); CHECK(m == Money{ -105, "USD" });
}
TEST_CASE("bad input") {
  Money m{ 1, "EUR" };
  for (const char* bad : { "abc EUR", "1.234 EUR", "5 EURO", "1.2.3 EUR" }) {
    std::istringstream in{ bad };
    CHECK_FALSE(static_cast<bool>(in >> m));
    CHECK(m == Money{ 1, "EUR" });
  }
}
TEST_CASE("round trip") {
  Money m{ -98765, "GBP" }, back{ 0, "XXX" };
  std::stringstream ss;
  ss << m;
  ss >> back;
  CHECK(back == m);
}
