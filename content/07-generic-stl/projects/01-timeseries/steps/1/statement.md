Write `template <typename T> class TimeSeries`:

- constructors: default, from an initialiser list, and from an iterator pair `TimeSeries(It first, It last)`
- `push_back`, `size`, `empty`, `operator[]` (const and non-const)
- member types `value_type`, `iterator`, `const_iterator`, and `begin()`/`end()` (const and non-const)

Then the series works with range-for, `std::max_element`, `std::accumulate` and every other algorithm, for `double`, `float` or `long double` alike.
