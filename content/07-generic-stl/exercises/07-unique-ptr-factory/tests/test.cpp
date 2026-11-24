#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include "factory.hpp"

TEST_CASE("parsing") {
  auto b = make_instrument("BOND 100 0.05");
  REQUIRE(b);
  CHECK(b->kind() == "BOND");
  CHECK(b->price() == doctest::Approx(105));
  CHECK(make_instrument("EQUITY 189.5")->price() == doctest::Approx(189.5));
  CHECK(make_instrument("SWAP 1") == nullptr);
  CHECK(make_instrument("BOND abc") == nullptr);
  CHECK(make_instrument("") == nullptr);
}
TEST_CASE("book value and ownership transfer") {
  long live = cpplab::live_allocations();
  int destroyed = instruments_destroyed();
  {
    std::vector<std::unique_ptr<Instrument>> book;
    book.push_back(make_instrument("EQUITY 50"));
    book.push_back(make_instrument("TRACKED 500"));
    book.push_back(make_instrument("BOND 100 0.1"));
    CHECK(book_value(book) == doctest::Approx(660));
    auto top = take_largest(book);
    REQUIRE(top);
    CHECK(top->price() == doctest::Approx(500));
    CHECK(book.size() == 2);
    CHECK(book_value(book) == doctest::Approx(160));
  }
  CHECK(instruments_destroyed() == destroyed + 1);
  CHECK(cpplab::live_allocations() == live);
}
TEST_CASE("empty book") {
  std::vector<std::unique_ptr<Instrument>> empty;
  CHECK(take_largest(empty) == nullptr);
}
