In the style of slides 159–161, write generic algorithms over a range `[first, last)`. They must work on a `vector`, a `list` **and** a raw array:

- `void for_each_(It first, It last, F f)`
- `std::size_t count_if_(It first, It last, P pred)`
- `T sum_(It first, It last, T init)`
- `It find_if_(It first, It last, P pred)`: `last` if nothing matches
