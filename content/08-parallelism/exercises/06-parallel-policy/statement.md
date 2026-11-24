Use **execution policies** (slide 209):

- `double sum_of_squares(const std::vector<double>& v)`: `std::transform_reduce` with `std::execution::par`
- `void normalize(std::vector<double>& v)`: subtract the mean from every element in parallel (`std::transform` with `std::execution::par`)
- `std::size_t count_outliers(const std::vector<double>& v, double threshold)`: elements with `|x| > threshold`, using `std::count_if` with `std::execution::par`
