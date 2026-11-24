Free function templates, implemented with STL algorithms:

- `TimeSeries<T> simple_returns(const TimeSeries<T>& prices)`: `p[i]/p[i-1] − 1`
- `TimeSeries<T> moving_average(const TimeSeries<T>& x, std::size_t window)`: the simple moving average, one value per full window (`size − window + 1` values; empty if too short). `window == 0` throws `std::invalid_argument`. Aim for O(n).
