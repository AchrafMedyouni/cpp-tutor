#include "doctest.h"
#include <sstream>
#include "Fraction.hpp"

std::string str(const Fraction& f) { std::ostringstream os; os << f; return os.str(); }

TEST_CASE("normalisation") {
  Fraction f{ 2, -4 };
  CHECK(f.num() == -1);
  CHECK(f.den() == 2);
  CHECK(Fraction{ 6, 3 } == Fraction{ 2 });
  CHECK_THROWS_AS(Fraction(1, 0), std::invalid_argument);
}
TEST_CASE("arithmetic") {
  CHECK(Fraction{ 1, 2 } + Fraction{ 1, 3 } == Fraction{ 5, 6 });
  CHECK(Fraction{ 1, 2 } - Fraction{ 3, 4 } == Fraction{ -1, 4 });
  CHECK(Fraction{ 2, 3 } * Fraction{ 9, 4 } == Fraction{ 3, 2 });
  CHECK(Fraction{ 1, 2 } / Fraction{ 1, 4 } == Fraction{ 2 });
  CHECK_THROWS_AS(Fraction{ 1 } / Fraction{ 0 }, std::invalid_argument);
  CHECK(1 + Fraction{ 1, 2 } == Fraction{ 3, 2 });   // implicit conversion from long
}
TEST_CASE("ordering") {
  CHECK(Fraction{ 1, 3 } < Fraction{ 1, 2 });
  CHECK(Fraction{ -1, 2 } < Fraction{ 1, 3 });
  CHECK(Fraction{ 2, 4 } >= Fraction{ 1, 2 });
}
TEST_CASE("printing") {
  CHECK(str(Fraction{ 3, 4 }) == "3/4");
  CHECK(str(Fraction{ 4, 2 }) == "2");
  CHECK(str(Fraction{ 1, -3 }) == "-1/3");
}
