#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include "linalg.hpp"

TEST_CASE("indexing and transpose") {
  Matrix m{ { 1, 2, 3 }, { 4, 5, 6 } };
  CHECK(m.rows() == 2);
  CHECK(m(1, 2) == 6);
  m(0, 0) = 7;
  Matrix t = m.transpose();
  CHECK(t.rows() == 3);
  CHECK(t(0, 0) == 7);
  CHECK(t(2, 1) == 6);
  CHECK_THROWS_AS(m(2, 0), std::out_of_range);
  CHECK_THROWS_AS((Matrix{ { 1, 2 }, { 3 } }), std::invalid_argument);
}
TEST_CASE("products") {
  Matrix a{ { 1, 2 }, { 3, 4 } }, id{ { 1, 0 }, { 0, 1 } };
  CHECK(a * id == a);
  CHECK(a * a == (Matrix{ { 7, 10 }, { 15, 22 } }));
  CHECK(a * Vector{ 1, 1 } == Vector{ 3, 7 });
  CHECK_THROWS_AS((a * Vector{ 1, 2, 3 }), std::invalid_argument);
}
