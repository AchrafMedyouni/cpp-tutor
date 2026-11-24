#include "doctest.h"
#include <iterator>
#include <list>
#include <string>
#include <vector>
#include "algos.hpp"

TEST_CASE("vector") {
  std::vector<int> v{ 1, 2, 3, 4 };
  for_each_(v.begin(), v.end(), [](int& x) { x *= 10; });
  CHECK(v[3] == 40);
  CHECK(count_if_(v.begin(), v.end(), [](int x) { return x > 15; }) == 3);
  CHECK(sum_(v.begin(), v.end(), 0) == 100);
  CHECK(find_if_(v.begin(), v.end(), [](int x) { return x == 30; }) == v.begin() + 2);
}
TEST_CASE("list and strings") {
  std::list<std::string> l{ "EUR", "USD", "JPY" };
  CHECK(sum_(l.begin(), l.end(), std::string{}) == "EURUSDJPY");
  CHECK(find_if_(l.begin(), l.end(), [](const std::string& s) { return s == "CHF"; }) == l.end());
}
TEST_CASE("raw array") {
  double px[] = { 1.5, 2.5, 3.0 };
  CHECK(sum_(std::begin(px), std::end(px), 0.0) == doctest::Approx(7.0));
}
