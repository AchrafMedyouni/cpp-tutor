Write a polymorphic payoff hierarchy:

- abstract `Payoff` with a virtual destructor and `virtual double operator()(double spot) const = 0;`
- `Call(K)`, `Put(K)`, `Digital(K, cash)` (pays `cash` if `spot > K`), and `Straddle(K)` (call + put)
- `double expected_payoff(const Payoff& p, const std::vector<double>& scenarios)`: the average payoff over the scenarios (0 if there are none)
