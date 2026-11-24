Given `enum class Side { Buy, Sell };`, write:

- `const char* to_string(Side s)`: `"BUY"` / `"SELL"`
- `Side opposite(Side s)`
- `int sign(Side s)`: `+1` for Buy, `-1` for Sell
- `Side parse_side(const std::string& text)`: accepts `"BUY"`, `"B"`, `"SELL"`, `"S"`, and throws `std::invalid_argument` for anything else
