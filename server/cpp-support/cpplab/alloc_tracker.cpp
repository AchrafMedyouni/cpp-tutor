// Replaces the global operator new/delete to count live allocations.
// Blocks go straight to malloc/free (no header), so AddressSanitizer still reports
// double frees, use-after-free, etc. at the user's own address.
#include "alloc_tracker.h"

#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <new>

namespace {
std::atomic<long> g_live{0};
std::atomic<long> g_total{0};

void* tracked_alloc(std::size_t n) {
  void* p = std::malloc(n == 0 ? 1 : n);
  if (!p) throw std::bad_alloc{};
  g_live.fetch_add(1, std::memory_order_relaxed);
  g_total.fetch_add(1, std::memory_order_relaxed);
  return p;
}

void tracked_free(void* p) noexcept {
  if (!p) return;
  g_live.fetch_sub(1, std::memory_order_relaxed);
  std::free(p);
}

struct ExitReport {
  ~ExitReport() {
    if (std::getenv("CPPLAB_TRACKER_QUIET")) return;
    long live = g_live.load();
    if (live > 0)
      std::fprintf(stderr, "\n[cpplab] %ld leaked allocation(s) (out of %ld allocations)\n", live, g_total.load());
    else
      std::fprintf(stderr, "\n[cpplab] no leaks (%ld allocations, all released)\n", g_total.load());
  }
};
// Destroyed after the user's globals because CppLab links this object file first
// (on Mach-O, init_priority is not honoured across translation units; link order is).
__attribute__((init_priority(101))) ExitReport g_report;
}  // namespace

namespace cpplab {
long live_allocations() { return g_live.load(); }
long total_allocations() { return g_total.load(); }
}  // namespace cpplab

void* operator new(std::size_t n) { return tracked_alloc(n); }
void* operator new[](std::size_t n) { return tracked_alloc(n); }
void* operator new(std::size_t n, const std::nothrow_t&) noexcept {
  try { return tracked_alloc(n); } catch (...) { return nullptr; }
}
void* operator new[](std::size_t n, const std::nothrow_t&) noexcept {
  try { return tracked_alloc(n); } catch (...) { return nullptr; }
}
void operator delete(void* p) noexcept { tracked_free(p); }
void operator delete[](void* p) noexcept { tracked_free(p); }
void operator delete(void* p, std::size_t) noexcept { tracked_free(p); }
void operator delete[](void* p, std::size_t) noexcept { tracked_free(p); }
void operator delete(void* p, const std::nothrow_t&) noexcept { tracked_free(p); }
void operator delete[](void* p, const std::nothrow_t&) noexcept { tracked_free(p); }
