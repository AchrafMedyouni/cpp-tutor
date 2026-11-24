#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include <algorithm>
#include <list>
#include <numeric>
#include <type_traits>
#include "timeseries.hpp"

TEST_CASE("container basics") {
  TimeSeries<double> ts{ 100, 101.5, 99 };
  ts.push_back(103);
  CHECK(ts.size() == 4);
  CHECK(ts[1] == 101.5);
  CHECK(std::is_same_v<TimeSeries<double>::value_type, double>);
  const TimeSeries<double>& c = ts;
  CHECK(*std::max_element(c.begin(), c.end()) == 103);
  CHECK(std::accumulate(ts.begin(), ts.end(), 0.0) == doctest::Approx(403.5));
}
TEST_CASE("generic over the element type, built from any range") {
  std::list<float> l{ 1.f, 2.f, 3.f };
  TimeSeries<float> f(l.begin(), l.end());
  float sum = 0;
  for (float x : f) sum += x;
  CHECK(sum == doctest::Approx(6.f));
  for (auto& x : f) x *= 2;
  CHECK(f[2] == doctest::Approx(6.f));
}
