#include "doctest.h"
#include <sstream>
#include "Money.hpp"

std::string str(const Money& m) { std::ostringstream os; os << m; return os.str(); }

TEST_CASE("arithmetic") {
  Money a{ 1050, "EUR" }, b{ 250, "EUR" };
  CHECK((a + b).cents() == 1300);
  CHECK((a - b).cents() == 800);
  CHECK((a * 3).cents() == 3150);
  CHECK((2 * b).cents() == 500);
  CHECK((a + b).currency() == "EUR");
}
TEST_CASE("currency mismatch") {
  Money e{ 100, "EUR" }, u{ 100, "USD" };
  CHECK_THROWS_AS(e + u, std::invalid_argument);
  CHECK_THROWS_AS((void)(e < u), std::invalid_argument);
  CHECK_FALSE(e == u);
}
TEST_CASE("ordering") { CHECK(Money{ 99, "EUR" } < Money{ 100, "EUR" }); }
TEST_CASE("printing") {
  CHECK(str(Money{ 1234, "EUR" }) == "12.34 EUR");
  CHECK(str(Money{ 5, "USD" }) == "0.05 USD");
  CHECK(str(Money{ -5, "USD" }) == "-0.05 USD");
  CHECK(str(Money{ -12300, "JPY" }) == "-123.00 JPY");
}
