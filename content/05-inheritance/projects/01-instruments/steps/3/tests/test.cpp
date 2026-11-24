#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include <type_traits>
#include "instruments.hpp"

TEST_CASE("future") { CHECK(Future{ "F", 0.5 }.price(Market{ 100, 0.04, 0.2 }) == doctest::Approx(102.020134)); }
TEST_CASE("book value and delta, no leaks") {
  long before = cpplab::live_allocations();
  {
    Book book;
    book.add(new Bond{ "B", 100, 0.05, 5 }, 10);
    book.add(new EuropeanOption{ "C", true, 100, 1.0 }, 2);
    book.add(new Future{ "F", 1.0 }, -1);
    Market m{ 100, 0.05, 0.2 };
    CHECK(book.size() == 3);
    double expected = 10 * Bond{ "", 100, 0.05, 5 }.price(m) + 2 * 10.450584 - 100 * std::exp(0.05);
    CHECK(book.value(m) == doctest::Approx(expected).epsilon(1e-6));
    // bond delta 0, call delta N(d1) = 0.636831, future delta e^{rT}
    CHECK(book.delta(m) == doctest::Approx(2 * 0.636831 - std::exp(0.05)).epsilon(1e-4));
  }
  CHECK(cpplab::live_allocations() == before);
}
TEST_CASE("a Book can't be copied") { CHECK_FALSE(std::is_copy_constructible_v<Book>); }
