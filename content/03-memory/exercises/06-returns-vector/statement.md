Write, taking the vectors by **const reference** (slide 73):

- `std::vector<double> log_returns(const std::vector<double>& prices)`: `ln(p[i] / p[i-1])`. The result has one element fewer than the input.
- `double sample_stdev(const std::vector<double>& x)`: divides by `n - 1`; returns 0 when `n < 2`
- `double max_drawdown(const std::vector<double>& prices)`: the largest peak-to-trough fall, returned as a **positive** fraction: prices `100, 120, 90` give `0.25` (a 25% fall from the 120 peak); a price series that only rises gives `0`
