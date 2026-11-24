---
title: "Producer–consumer and condition variables"
slides: "204–208"
---

The **producer** puts data into a fixed-size mailbox, and the **consumer** takes it out (slide 204). They must synchronise: the consumer waits until there is data, and the producer waits until there is room. Variants have several producers and consumers. In trading, the producer is a market-data feed handler and the consumer is a strategy.

Polling a flag in a loop would be *busy waiting*. A **`std::condition_variable`** lets a thread **block** until another thread notifies it:

```cpp
class condition_variable {
public:
  void notify_one();
  void notify_all();
  void wait( unique_lock< mutex > & lock );                 // atomically: unlock, sleep, relock on wake-up
  template< class Pred > void wait( unique_lock< mutex > & lock, Pred stop_waiting );
  bool wait_for( ... ); bool wait_until( ... );
};
```

`wait` must **release the mutex while sleeping**, which is why it takes a `unique_lock`: a `lock_guard` can't be unlocked.

The slides' version (slides 206–208) has a mailbox holding **one** item, a `data_in_box` flag and two condition variables:

::example{id="producer-consumer-slides"}

:::course
Slides 207–208 write `if( data_in_box ) box_empty.wait( lock );`. A thread may wake up **without** having been notified (a *spurious wakeup*, which the standard explicitly allows), or another consumer may have taken the item first. The condition must be **re-checked in a loop**: `while( data_in_box ) box_empty.wait( lock );`, or with the predicate overload: `box_empty.wait( lock, []{ return !data_in_box; } );`.
:::

A reusable, correct version: a bounded queue template, used by a market-data pipeline.

::example{id="bounded-queue"}
