---
title: "Comparisons and <=> (C++20)"
slides: "136"
---

`<=>` (the *three-way comparison*, or "spaceship") appears in slide 136's table. Before C++20, a comparable type needed up to six hand-written operators (`== != < <= > >=`). Now:

```cpp
struct Version {
  int major, minor;
  auto operator<=>( const Version & ) const = default;   // compares members in order
  bool operator==( const Version & ) const = default;    // (implied by a defaulted <=>)
};
```

- `a <=> b` returns an *ordering* (`std::strong_ordering::less/equal/greater`, or `partial_ordering` for floating point, where NaN is unordered).
- The compiler **rewrites** `a < b` as `(a <=> b) < 0`, and `a != b` as `!(a == b)`, so one or two declarations give you all six operators.
- `= default` compares the members **lexicographically, in declaration order**, like comparing Python tuples.

Write `<=>` yourself when the natural order isn't member-wise. For example, in an order book, bids are ranked by **highest price first, then earliest time**:

::example{id="spaceship"}

:::finance
This is how price-time priority is expressed when you put orders into a `std::set` or `std::priority_queue`, which chapter 7's order-book project does.
:::
