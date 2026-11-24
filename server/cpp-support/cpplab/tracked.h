// CppLab instrumented type: counts how it is constructed, copied, moved and destroyed.
// Used by exercise tests to check behaviour (e.g. "moved, not copied", "destructor ran N times").
#pragma once
#include <ostream>

namespace cpplab {

struct Counters {
  int ctor = 0, copy_ctor = 0, move_ctor = 0, copy_assign = 0, move_assign = 0, dtor = 0;
  int alive() const { return ctor + copy_ctor + move_ctor - dtor; }
};

class Tracked {
 public:
  static inline Counters counts{};
  static void reset() { counts = Counters{}; }

  Tracked() : value_{0} { ++counts.ctor; }
  explicit Tracked(int v) : value_{v} { ++counts.ctor; }
  Tracked(const Tracked& o) : value_{o.value_} { ++counts.copy_ctor; }
  Tracked(Tracked&& o) noexcept : value_{o.value_} { o.value_ = -1; ++counts.move_ctor; }
  Tracked& operator=(const Tracked& o) { value_ = o.value_; ++counts.copy_assign; return *this; }
  Tracked& operator=(Tracked&& o) noexcept { value_ = o.value_; o.value_ = -1; ++counts.move_assign; return *this; }
  ~Tracked() { ++counts.dtor; }

  int value() const { return value_; }
  void set(int v) { value_ = v; }
  friend bool operator==(const Tracked& a, const Tracked& b) { return a.value_ == b.value_; }
  friend std::ostream& operator<<(std::ostream& os, const Tracked& t) { return os << "Tracked(" << t.value_ << ")"; }

 private:
  int value_;
};

}  // namespace cpplab
