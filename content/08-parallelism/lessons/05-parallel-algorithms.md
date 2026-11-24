---
title: "Parallel algorithms and oneTBB"
slides: "209–213"
---

## Execution policies (C++17)

69 of the standard algorithms accept an **execution policy** as their first argument (`<execution>`):

| policy | meaning |
|---|---|
| `std::execution::seq` | sequential |
| `std::execution::par` | parallel, on several threads |
| `std::execution::unseq` | vectorised (SIMD) on one thread (C++20) |
| `std::execution::par_unseq` | parallel **and** vectorised |

```cpp
std::for_each( std::execution::par, v.begin(), v.end(), []( int & n ){ n++; } );
```

The callable must be **safe to run concurrently**: no data races, so no unsynchronised shared writes. With `par_unseq`, it also can't take locks.

Implementations (slide 210): g++ ≥ 9 (uses oneTBB), MSVC ≥ 2017, HPX, oneTBB itself, NVIDIA Thrust (GPU). With libc++ (Apple clang), CppLab adds `-fexperimental-library` automatically. If your setup has no backend, the examples below show a notice instead.

::example{id="execution-par"}

## oneTBB (optional)

Intel's **oneAPI Threading Building Blocks** is a task-based library: you describe the work and its scheduler balances it across cores with work stealing. The slides convert a serial loop (slide 211) to `parallel_for` over a `blocked_range`, first with a functor (slide 212), then with a lambda (slide 213):

```cpp
void SerialApplyFoo( float a[], size_t n ) { for( size_t i{ 0 }; i != n; ++i ) Foo( a[i] ); }

void parallelApplyFoo( float a[], size_t n ) {
  parallel_for( size_t( 0 ), n, [=]( size_t i ) { Foo( a[i] ); } );
}
```

oneTBB is **optional** in CppLab. Install it with `brew install tbb` (macOS) or `sudo apt install libtbb-dev` (Linux), then restart `npm run dev`. The sidebar shows `tbb: ✓` once it is detected.

::example{id="tbb-parallel-for"}
