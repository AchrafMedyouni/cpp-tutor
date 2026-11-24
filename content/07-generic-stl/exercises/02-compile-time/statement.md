Like `Factorial<N>` (slide 152), compute at compile time:

- `template <unsigned N> struct Fib { static constexpr unsigned long long value = ...; };` with `Fib<0> = 0`, `Fib<1> = 1`
- `constexpr unsigned long long fib(unsigned n)`: the same thing, written as a `constexpr` function
- `template <unsigned long long B, unsigned E> struct Power { static constexpr unsigned long long value = ...; };` (a partial specialisation for `E = 0`)

The tests use `static_assert`, so a wrong value fails **at compile time**.
