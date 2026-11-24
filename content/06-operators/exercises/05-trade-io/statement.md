Given `struct Trade { std::string symbol; bool buy; long qty; double price; };`, write (slides 144–145):

- `std::ostream& operator<<(std::ostream&, const Trade&)`: `AAPL BUY 100 @ 189.50` (2 decimals). It must not leave the stream in `fixed` mode.
- `std::istream& operator>>(std::istream&, Trade&)`: parses that same format. On any error (bad side, missing `@`, bad number), it sets `failbit` and leaves the `Trade` unchanged.

Then `while (in >> t)` reads a whole blotter.
