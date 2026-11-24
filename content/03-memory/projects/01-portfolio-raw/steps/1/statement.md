A `Portfolio` is a plain struct holding a **raw dynamic array** of `Position`s (slides 68, 76).

- `Portfolio make_portfolio(int capacity)`: allocates `capacity` positions with `new[]`; `size` starts at 0
- `void destroy(Portfolio& p)`: releases the array with `delete[]` and resets the fields

The tests count heap allocations: after `destroy`, nothing may remain.
