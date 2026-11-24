Use `using Payoff = std::function<double(double)>;` (spot → payoff at expiry). Write factories that return lambdas:

- `Payoff make_call(double K)`: `max(S - K, 0)`
- `Payoff make_put(double K)`: `max(K - S, 0)`
- `Payoff make_bull_spread(double K1, double K2)`: long call K1, short call K2 (K1 < K2)
- `Payoff combine(Payoff a, Payoff b, double wa, double wb)`: `wa·a(S) + wb·b(S)`

(The type alias is already in the starter.)
