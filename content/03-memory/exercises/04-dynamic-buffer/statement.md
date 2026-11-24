Manage a raw dynamic array by hand (slide 76):

- `double* make_prices(int n, double start, double step)`: a new array `{start, start+step, …}`
- `double average(const double* p, int n)`
- `double* grow(double* old, int old_n, int new_n)`: returns a **new** array of `new_n` elements. The first `old_n` are copied and the rest set to 0. The old array is released.
- `void release(double* p)`

The tests run under ASan and **count heap allocations**: a leak or a double free fails.
