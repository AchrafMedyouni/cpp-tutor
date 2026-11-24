#pragma once
#include <cstddef>

template <typename It, typename F>
void for_each_(It first, It last, F f) { (void)first; (void)last; (void)f; }   // TODO

template <typename It, typename P>
std::size_t count_if_(It first, It last, P pred) { (void)first; (void)last; (void)pred; return 0; }   // TODO

template <typename It, typename T>
T sum_(It first, It last, T init) { (void)first; (void)last; return init; }   // TODO

template <typename It, typename P>
It find_if_(It first, It last, P pred) { (void)first; (void)pred; return last; }   // TODO
