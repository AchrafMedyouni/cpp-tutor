Two classic money-handling problems:

- `std::vector<Money> split(const Money& m, int n)`: split into `n` parts that **add up exactly** to `m` and differ by at most one cent. The first parts get the extra cents. (100.00 EUR / 3 = 33.34 + 33.33 + 33.33.) Throws `std::invalid_argument` if `n <= 0`.
- `Money apply_bps(const Money& m, double bps)`: a fee of `bps` basis points (1 bp = 0.01%), rounded **half away from zero** to the cent
