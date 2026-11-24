- `operator<<` prints `12.34 EUR`, `-0.05 USD`, `7.00 JPY`
- `operator>>` reads the same format, and also accepts `7 JPY` or `0.5 EUR` (meaning 0.50). On malformed input (`abc EUR`, `1.234 EUR`, a currency that isn't 3 letters) it sets `failbit` and leaves the target untouched.

A round trip `out << m; in >> m2;` must give back `m2 == m`.
