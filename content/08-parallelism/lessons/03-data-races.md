---
title: "Shared data, races and mutual exclusion"
slides: "194–203"
---

## A shared variable (KO)

Two threads both run `x += 5` on the same `int`:

::example{id="data-race"}

Why can the result be wrong? `x += 5` is **not one operation**. The CPU *loads* `x` into a register, *adds* 5, and *stores* it back (slides 195–196). If the scheduler switches threads between the load and the store, an update is lost:

| step | t1 | t2 | `n` in memory |
|---|---|---|---|
| 1 | `LDR n` → reg = 2 | | 2 |
| 2 | `ADD 5` → reg = 7 | | 2 |
| 3 | *preempted* | `LDR n` → reg = 2 | 2 |
| 4 | | `ADD 5` → reg = 7 | 2 |
| 5 | | `STR n` | 7 |
| 6 | `STR n` | | **7** (should be 12) |

In C++ a **data race** (two threads accessing the same variable, at least one writing, without synchronisation) is **undefined behaviour**, not just a wrong number. ThreadSanitizer (*TSan*) detects races at run time.

## Definitions and requirements

- **Critical resource**: one that must not be accessed by several threads at once.
- **Critical section**: the code that accesses a critical resource.
- **Mutual exclusion**: a mechanism that lets only one thread at a time into a critical section.

A good mechanism provides (slide 198):
- **safety**: at most one thread uses the resource at any time;
- **liveness**: a free resource is granted immediately, and a waiting thread gets it within finite time;
- **efficiency**: a waiting thread doesn't use the CPU (it is *blocked*).

Software-only algorithms exist (Peterson's, Dekker's), but they **busy-wait**. Only the OS can put a thread in the blocked state, so the OS has to provide the primitive.

## `std::mutex`, `lock_guard`, `unique_lock`

A **mutex** (`<mutex>`) has `lock()`, `try_lock()` and `unlock()`. Variants: `recursive_mutex` (the same thread may lock it several times), `timed_mutex` (`try_lock_for/until`). Calling `lock`/`unlock` by hand is fragile: an exception or an early `return` leaves the mutex locked forever. Use RAII instead:
- **`std::lock_guard<Mutex>`** locks in its constructor and unlocks in its destructor. Nothing else.
- **`std::unique_lock<Mutex>`** does the same, but can also unlock and relock, try, time out and be moved. Condition variables need it.

::example{id="mutex-counter"}

## `std::atomic`

For a single variable, **`std::atomic<T>`** (`<atomic>`, e.g. `std::atomic_int`) turns `+=`, `++`, loads and stores into **indivisible** hardware operations. No lock is needed, and it is usually faster than a mutex for counters and flags.

::example{id="atomic-counter"}

## Deadlock

Two mutexes taken in **opposite orders** by two threads can deadlock: each holds one and waits forever for the other (a *circular wait*). On Linux, TSan reports such a *lock-order inversion* even on runs where the timing happened to work out. That detector isn't available on macOS. The example below forces the bad interleaving and uses `timed_mutex::try_lock_for`, so that it reports the deadlock instead of hanging:

::example{id="deadlock"}

:::modern
To take several locks at once, use `std::scoped_lock lock{ m1, m2 };` (C++17). It locks all of them with a deadlock-avoidance algorithm. Otherwise, always acquire locks in one global order.
:::
