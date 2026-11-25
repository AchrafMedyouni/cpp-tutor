Implement each function with the **standard algorithms** (`<algorithm>`, `<numeric>`) rather than raw index loops (slide 167):

- `std::vector<double> simple_returns(const std::vector<double>& p)`: `p[i]/p[i-1] - 1`
- `double mean(const std::vector<double>& x)`: 0 when empty
- `std::vector<double> top_n(const std::vector<double>& x, std::size_t n)`: the `n` largest, in descending order (fewer if `x` is shorter)
- `std::size_t count_up_days(const std::vector<double>& p)`: how many times the price rose
- `double max_drawdown(const std::vector<double>& p)`: as in chapter 3, a **positive** fraction (`100, 120, 90` → `0.25`)
