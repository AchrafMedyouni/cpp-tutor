Write `class Buffer`, a tiny string type that owns a `char*` allocated with `new[]` (slides 98–114):

- `explicit Buffer(const char* s = "")`, `std::size_t size() const`, `const char* c_str() const`
- `void append(const char* s)`
- the **rule of three**: destructor, copy constructor (deep copy), copy assignment (copy-and-swap)

The tests check **independence** of copies, **self-assignment**, and that no allocation leaks (ASan + allocation counter). `<cstring>` gives you `std::strlen` and `std::memcpy`.
