---
title: "std::thread"
slides: "192–193"
---

`std::thread` (header `<thread>`) starts a new thread **in its constructor**. The thread runs any callable with the given arguments:

```cpp
class thread {
public:
  template< class Function, class... Args >
  explicit thread( Function&& f, Args&&... args );
  void join();      // wait for the thread to finish
  void detach();    // let it run on its own (rarely what you want)
  id get_id() const;
};
namespace this_thread { id get_id(); void sleep_for( ... ); void sleep_until( ... ); }
```

- Arguments are **copied** into the thread. To pass a reference, wrap it in **`std::ref(x)`** (slide 193).
- Every `std::thread` must be `join()`ed or `detach()`ed before it is destroyed, otherwise the program calls `std::terminate`. **C++20's `std::jthread`** joins automatically in its destructor (RAII again) and can be asked to stop.

::example{id="thread-basics"}

::example{id="missing-ref"}

::example{id="jthread"}
