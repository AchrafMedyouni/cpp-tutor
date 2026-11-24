#pragma once

template <unsigned N>
struct Fib { static constexpr unsigned long long value = 0; };   // TODO

constexpr unsigned long long fib(unsigned n) { (void)n; return 0; }   // TODO

template <unsigned long long B, unsigned E>
struct Power { static constexpr unsigned long long value = 0; };   // TODO
