---
title: "Algorithms and function objects"
slides: "158–163"
---

Slides 158–162 build an STL algorithm step by step:

1. `mettre_a_1( Container & c )`: generic over the container type (range-for inside).
2. `mettre_a_1( Iter begin, Iter end )`: generic over a **range** of any iterators. It now works on part of a container, on raw arrays, on lists…
3. `mettre_a_val( Iter beg, Iter end, T val )`: the value becomes a parameter.
4. `apply( Iter beg, Iter end, const F & fun )`: the **operation** becomes a parameter. Any callable works: a lambda, a function pointer, or…
5. …a **function object** (*functor*): a struct with `operator()` and some state.

::example{id="generalization"}

That's exactly how the standard library is designed. Its **four main concepts** (slide 163):

| concept | role |
|---|---|
| **containers** | store objects. They differ in the time complexity of their operations |
| **iterators** | traverse the objects of a container |
| **algorithms** | process a range given by two iterators |
| **function objects** | specify what the algorithms do (lambdas included) |

A fifth one, **adaptors**, changes the interface or behaviour of containers and iterators (`stack`, `reverse_iterator`, `back_inserter`).

The headers are `<algorithm>`, plus `<numeric>` for `accumulate`, `partial_sum` and similar. Families (slide 167):
- **non-modifying:** `for_each`, `count`, `count_if`, `find`, `all_of`, `min_element`, `accumulate`…
- **modifying:** `copy`, `move`, `fill`, `transform`…
- **partitioning and sorting:** `partition`, `sort`, `stable_sort`…
- **on sorted ranges:** `binary_search`, `lower_bound`, `set_union`…

::example{id="stl-algorithms"}

:::finance
Replacing raw loops with named algorithms makes intent obvious: `max_element` *is* "the highest price". Algorithms also become parallel with one extra argument in chapter 8 (`std::execution::par`).
:::
