// CppLab allocation tracker: counts live heap allocations made through operator new.
// Linked automatically when "Track allocations" is on, and always in graded exercises.
#pragma once
#include <cstddef>

namespace cpplab {
// Number of blocks allocated with new / new[] and not yet released.
long live_allocations();
// Total number of calls to operator new / new[] since program start.
long total_allocations();
}  // namespace cpplab
