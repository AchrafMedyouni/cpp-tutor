The functions in `positions.hpp` compute the right values, but their **signatures** are wrong: the tests (which you can't change) pass **const** data and temporaries, so nothing compiles.

Fix the parameter and return types (`const` placement, pointer vs reference) so that the tests compile and pass. Don't change what the functions compute.

**This is exactly how the tests call your functions:**

```cpp
const Position book[] = { {10, 5.0}, {2, 100.0}, {30, 1.0} };   // a CONST array

total(book, 3);                          // must return 280
notional(Position{3, 2.5});              // a temporary (rvalue): must return 7.5
const Position* b = best(book, 3);       // must point to book[1]

Position p{100, 1.0};                    // a normal, non-const variable
scale(p, 0.5);                           // afterwards p.qty must be 50
```

For each call, ask: *what type is the argument, and can the current parameter type accept it?*
