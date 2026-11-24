#include "doctest.h"
#include <type_traits>
#include "formes.hpp"

TEST_CASE("Forme is abstract with a virtual destructor") {
  CHECK(std::is_abstract_v<Forme>);
  CHECK(std::has_virtual_destructor_v<Forme>);
}
TEST_CASE("areas and perimeters") {
  Cercle c{ 1 };
  Rectangle r{ 2, 3 };
  Carre s{ 4 };
  CHECK(c.aire() == doctest::Approx(3.14159265));
  CHECK(r.perimetre() == doctest::Approx(10));
  CHECK(s.aire() == doctest::Approx(16));
  CHECK(s.perimetre() == doctest::Approx(16));
}
TEST_CASE("total_area uses dynamic binding") {
  Cercle c{ 1 };
  Rectangle r{ 2, 3 };
  Carre s{ 2 };
  CHECK(total_area({ &c, &r, &s }) == doctest::Approx(3.14159265 + 6 + 4));
}
