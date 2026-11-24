Value the book against market prices, given as two **parallel arrays** (C style: a pointer plus a length):

- `double market_value(const Portfolio& p, const std::string* symbols, const double* marks, int n)`: `Σ qty · mark` over the positions that have a mark (ignore the others)
- `double unrealized_pnl(Portfolio& p, const std::string& symbol, double mark)`: `qty · (mark − avg_price)`, or 0 for an unknown symbol
