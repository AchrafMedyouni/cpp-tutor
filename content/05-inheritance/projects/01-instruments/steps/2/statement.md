Add `EuropeanOption(name, bool is_call, double strike, double maturity)`, priced with **Black–Scholes** using `m.spot`, `m.rate` (continuously compounded) and `m.vol`.

`std::erfc` (in `<cmath>`) gives you the normal CDF: `N(x) = ½·erfc(−x/√2)`. The tests also check **put–call parity**: `C − P = S − K·e^{−rT}`.
