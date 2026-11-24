An id generator has an **identity**: two copies would hand out duplicate ids. Write `class IdGenerator` (slide 115):

- `explicit IdGenerator(std::string prefix)`
- `std::string next()`: `"ORD-1"`, `"ORD-2"`, … for prefix `"ORD-"`
- `long issued() const`
- copy construction and copy assignment **deleted**
