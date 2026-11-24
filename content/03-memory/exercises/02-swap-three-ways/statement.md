Write:

- `void swap_ptr(int* a, int* b)`: passing by address (slide 63)
- `void swap_ref(int& a, int& b)`: passing by reference (slide 72)
- `void rotate3(int& a, int& b, int& c)`: `(a, b, c)` becomes `(b, c, a)`
- `void apply_fill(long& position, double& cash, long qty, double price)`: a buy if `qty > 0`, a sell if `qty < 0`. Cash decreases by `qty · price`.
