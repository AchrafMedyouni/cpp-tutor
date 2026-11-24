Write three **function templates** (slide 150), working for any type with the needed operators:

- `T max_of(const std::vector<T>& v)`: throws `std::invalid_argument` if `v` is empty
- `T clamp_to(T v, T lo, T hi)`: uses only `<`
- `std::size_t count_equal(const std::vector<T>& v, const T& x)`

The tests use them with `int`, `double` and `std::string`.
