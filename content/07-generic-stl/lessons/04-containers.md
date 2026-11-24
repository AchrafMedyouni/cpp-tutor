---
title: "A tour of the containers"
slides: "164–166"
---

| Container | Structure | Access | Insert/erase | Notes |
|---|---|---|---|---|
| `vector<T>` | contiguous, growable | `[i]` O(1) | O(1) amortised at the end, O(n) elsewhere | **the default choice** |
| `array<T, N>` | contiguous, fixed size | O(1) | n/a | no heap allocation |
| `deque<T>` | blocks | O(1) | O(1) at both ends | |
| `list<T>` / `forward_list<T>` | doubly or singly linked | sequential only | O(1) anywhere, given an iterator | poor cache locality |
| `map<K, V>` / `set<K>` | balanced tree, **sorted** | O(log n) | O(log n) | ordered iteration |
| `multimap` / `multiset` | same, duplicate keys allowed | | | |
| `unordered_map<K, V>` / `unordered_set` (+ multi) | hash table | O(1) average | O(1) average | like Python `dict` / `set` |
| `stack<T>`, `queue<T>`, `priority_queue<T>` | **adaptors** over another container | top/front only | | restricted interfaces |

Every container has the same core interface (slide 165): the types `value_type`, `iterator` and `const_iterator`, and the methods `size()`, `empty()`, `clear()`, `begin()`, `end()`, `cbegin()` and `cend()`.

**Iterator categories** (slide 166) say what an iterator can do, and therefore which algorithms accept it:
- **forward** (`*`, `++`); input and output iterators are its read-only and write-only cousins;
- **bidirectional**: also `--` (`list`, `map`, `set`);
- **random access**: also `it + n`, `it - n`, `it[n]` (`vector`, `deque`, `array`, raw pointers).

`std::sort` needs random access, so it doesn't accept a `std::list`. Lists have their own `.sort()`.

Iterator **adaptors**: `reverse_iterator` (`rbegin()`/`rend()`), and the insert iterators `back_inserter`, `front_inserter` and `inserter`, which turn a write into an insertion.

::example{id="containers-tour"}

::example{id="positions-map"}

::example{id="priority-queue"}

::example{id="sort-list"}
