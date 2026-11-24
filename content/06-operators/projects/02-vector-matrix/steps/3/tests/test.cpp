#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include <cmath>
#include <sstream>
#include "linalg.hpp"

TEST_CASE("printing") {
  std::ostringstream os;
  os << Vector{ 1, 2.5, 3 } << "\n" << Matrix{ { 1, 0 }, { 0, 1 } };
  CHECK(os.str() == "[1, 2.5, 3]\n[1, 0]\n[0, 1]\n");
}
TEST_CASE("portfolio variance") {
  double s1 = 0.2, s2 = 0.3, rho = 0.5;
  Matrix cov{ { s1 * s1, rho * s1 * s2 }, { rho * s1 * s2, s2 * s2 } };
  Vector w{ 0.6, 0.4 };
  double var = portfolio_variance(cov, w);
  CHECK(var == doctest::Approx(0.0432));
  CHECK(std::sqrt(var) == doctest::Approx(0.207846));
  CHECK_THROWS_AS((portfolio_variance(Matrix{ 2, 3 }, Vector{ 1, 1, 1 })), std::invalid_argument);
}
