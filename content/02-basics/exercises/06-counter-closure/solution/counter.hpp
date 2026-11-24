#pragma once
#include <functional>

inline std::function<long()> make_id_generator(long start, long step) {
  return [next = start, step]() mutable {
    long id = next;
    next += step;
    return id;
  };
}
