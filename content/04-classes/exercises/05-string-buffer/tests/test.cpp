#include "doctest.h"
#include <string>
#include "cpplab/alloc_tracker.h"
#include "Buffer.hpp"

TEST_CASE("construct and append") {
  Buffer b{ "EUR" };
  CHECK(b.size() == 3);
  b.append("/USD");
  CHECK(std::string(b.c_str()) == "EUR/USD");
  CHECK(b.size() == 7);
}
TEST_CASE("copies are independent") {
  Buffer a{ "AAPL" };
  Buffer b{ a };
  b.append(".O");
  CHECK(std::string(a.c_str()) == "AAPL");
  CHECK(std::string(b.c_str()) == "AAPL.O");
}
TEST_CASE("assignment and self-assignment") {
  Buffer a{ "one" }, b{ "two two" };
  a = b;
  b.append("!");
  CHECK(std::string(a.c_str()) == "two two");
  Buffer& same = a;
  a = same;
  CHECK(std::string(a.c_str()) == "two two");
}
TEST_CASE("no leaks") {
  long before = cpplab::live_allocations();
  {
    Buffer a{ "x" };
    Buffer b{ a };
    Buffer c;
    c = a;
    c.append("yz");
    a = c;
  }
  CHECK(cpplab::live_allocations() == before);
}
