#include "doctest.h"
#include "complexe.hpp"

TEST_CASE("module") { CHECK(module({ 3, 4 }) == doctest::Approx(5)); }
TEST_CASE("conjugate") {
  Complexe c = conjugate({ 1, -2 });
  CHECK(c.real == 1);
  CHECK(c.imag == 2);
}
TEST_CASE("add") {
  Complexe c = add({ 1, 2 }, { 0.5, -4 });
  CHECK(c.real == doctest::Approx(1.5));
  CHECK(c.imag == doctest::Approx(-2));
}
TEST_CASE("approx_equal") {
  CHECK(approx_equal({ 0.1 + 0.2, 1 }, { 0.3, 1 }));
  CHECK_FALSE(approx_equal({ 1, 1 }, { 1, 1.1 }));
  CHECK(approx_equal({ 1, 1 }, { 1, 1.1 }, 0.2));
}
