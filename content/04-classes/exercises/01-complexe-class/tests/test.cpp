#include "doctest.h"
#include <type_traits>
#include "Complexe.hpp"

double mod_of(const Complexe& c) { return c.module(); }   // needs const methods

TEST_CASE("constructors and getters") {
  Complexe z;
  CHECK(z.real() == 0);
  CHECK(z.imag() == 0);
  const Complexe c{ 3, -4 };
  CHECK(c.real() == 3);
  CHECK(c.imag() == -4);
  CHECK(mod_of(c) == doctest::Approx(5));
}
TEST_CASE("setters") {
  Complexe c{ 1, 1 };
  c.real(6);
  c.imag(8);
  CHECK(c.module() == doctest::Approx(10));
}
TEST_CASE("ajoute") {
  Complexe a{ 1, 2 };
  a.ajoute(Complexe{ 2, 3 });
  CHECK(a.real() == 3);
  CHECK(a.imag() == 5);
}
