#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include "linalg.hpp"

TEST_CASE("construction and indexing") {
  Vector z(3);
  CHECK(z.size() == 3);
  CHECK(z[2] == 0);
  Vector v{ 1, 2, 3 };
  v[0] = 10;
  const Vector& c = v;
  CHECK(c[0] == 10);
  CHECK_THROWS_AS((void)c[3], std::out_of_range);
}
TEST_CASE("arithmetic") {
  Vector a{ 1, 2 }, b{ 3, 4 };
  CHECK(a + b == Vector{ 4, 6 });
  CHECK(b - a == Vector{ 2, 2 });
  CHECK(2.0 * a == Vector{ 2, 4 });
  CHECK(a * 0.5 == Vector{ 0.5, 1 });
  CHECK(dot(a, b) == doctest::Approx(11));
  CHECK_THROWS_AS((a + Vector{ 1, 2, 3 }), std::invalid_argument);
}
