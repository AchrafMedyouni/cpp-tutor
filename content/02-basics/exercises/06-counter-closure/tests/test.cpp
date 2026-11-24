#include "doctest.h"
#include "counter.hpp"

TEST_CASE("sequence") {
  auto gen = make_id_generator(100, 10);
  CHECK(gen() == 100);
  CHECK(gen() == 110);
  CHECK(gen() == 120);
}
TEST_CASE("generators are independent") {
  auto a = make_id_generator(1, 1);
  auto b = make_id_generator(1000, 5);
  CHECK(a() == 1);
  CHECK(b() == 1000);
  CHECK(a() == 2);
  CHECK(b() == 1005);
}
TEST_CASE("copying a generator copies its state") {
  auto a = make_id_generator(0, 2);
  a();
  auto b = a;          // b continues from a's current state, independently
  CHECK(a() == 2);
  CHECK(b() == 2);
}
