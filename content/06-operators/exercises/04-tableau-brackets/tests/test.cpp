#include "doctest.h"
#include <type_traits>
#include "Tableau.hpp"

double read(const Tableau& t, unsigned i) { return t[i]; }

TEST_CASE("write and read") {
  Tableau t{ 4 };
  for (unsigned i = 0; i < 4; ++i) t[i] = i * 1.5;
  CHECK(read(t, 3) == doctest::Approx(4.5));
  CHECK(t.sum() == doctest::Approx(9));
}
TEST_CASE("const indexing returns a const reference") {
  const Tableau t{ 2 };
  CHECK(std::is_same_v<decltype(t[0]), const double&>);
}
TEST_CASE("bounds") {
  Tableau t{ 2 };
  const Tableau& c = t;
  CHECK_THROWS_AS(t[2] = 1, std::out_of_range);
  CHECK_THROWS_AS((void)c[5], std::out_of_range);
}
