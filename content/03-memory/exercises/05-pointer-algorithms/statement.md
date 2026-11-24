Write these **without** using indices (`[]`). Use only pointers, `*`, `++`, `--` and comparisons (slides 77–79). Ranges are half-open: `[first, last)`.

- `void reverse(int* first, int* last)`
- `int* find_first(int* first, int* last, int value)`: `last` if not found
- `long sum(const int* first, const int* last)`
- `const int* max_pos(const int* first, const int* last)`: the first maximum, or `last` if the range is empty
