Given `struct Fill { std::string symbol; long qty; double price; };`, write:

- `std::map<std::string, double> vwap_by_symbol(const std::vector<Fill>& fills)`: the volume-weighted average price per symbol. A `std::map` keeps the symbols sorted.
- `std::vector<std::string> most_traded(const std::vector<Fill>& fills, std::size_t k)`: the `k` symbols with the largest total quantity, highest first. Ties are broken alphabetically.
