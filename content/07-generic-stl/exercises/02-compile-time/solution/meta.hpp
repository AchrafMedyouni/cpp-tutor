#pragma once

template <unsigned N>
struct Fib { static constexpr unsigned long long value = Fib<N - 1>::value + Fib<N - 2>::value; };
template <>
struct Fib<0> { static constexpr unsigned long long value = 0; };
template <>
struct Fib<1> { static constexpr unsigned long long value = 1; };

constexpr unsigned long long fib(unsigned n) {
  unsigned long long a = 0, b = 1;
  for (unsigned i = 0; i < n; ++i) {
    unsigned long long t = a + b;
    a = b;
    b = t;
  }
  return a;
}

template <unsigned long long B, unsigned E>
struct Power { static constexpr unsigned long long value = B * Power<B, E - 1>::value; };
template <unsigned long long B>
struct Power<B, 0> { static constexpr unsigned long long value = 1; };
