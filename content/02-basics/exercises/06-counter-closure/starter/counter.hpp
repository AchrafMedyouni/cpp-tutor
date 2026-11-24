#pragma once
#include <functional>

inline std::function<long()> make_id_generator(long start, long step) {
  (void)step;
  return [start]() { return start; };   // TODO: must advance
}
