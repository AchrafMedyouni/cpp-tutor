- `Position* find(Portfolio& p, const std::string& symbol)`: a pointer to the position, or `nullptr` (slide 60). Walk the array with a **pointer**.
- `void add_fill(Portfolio& p, const std::string& symbol, long qty, double price)`: `qty > 0` buys and `qty < 0` sells.
  - A new symbol gets a new position. If the array is full, **grow** it: allocate double the capacity, copy, and `delete[]` the old array.
  - On a buy, the average price becomes the weighted average of the old and new quantities. A sell leaves it unchanged.

The tests add more symbols than the initial capacity, and check for leaks and use-after-free (ASan).
