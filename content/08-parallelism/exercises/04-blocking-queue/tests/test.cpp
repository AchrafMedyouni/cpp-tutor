#include "doctest.h"
#include <atomic>
#include <thread>
#include <vector>
#include "queue.hpp"

TEST_CASE("FIFO, single thread") {
  BlockingQueue<int> q{ 4 };
  CHECK(q.push(1));
  CHECK(q.push(2));
  CHECK(q.size() == 2);
  CHECK(*q.pop() == 1);
  q.close();
  CHECK(*q.pop() == 2);           // drained after close
  CHECK_FALSE(q.pop().has_value());
  CHECK_FALSE(q.push(3));
}
TEST_CASE("producers and consumers") {
  BlockingQueue<long> q{ 8 };
  std::atomic<long> total{ 0 }, received{ 0 };
  std::vector<std::thread> consumers;
  for (int c = 0; c < 3; ++c)
    consumers.emplace_back([&] {
      while (auto v = q.pop()) { total += *v; ++received; }
    });
  std::vector<std::thread> producers;
  for (int p = 0; p < 2; ++p)
    producers.emplace_back([&q, p] { for (long i = 1; i <= 1000; ++i) q.push(i + p * 1000); });
  for (auto& t : producers) t.join();
  q.close();
  for (auto& t : consumers) t.join();
  CHECK(received == 2000);
  CHECK(total == 2000L * 2001 / 2);
}
TEST_CASE("close wakes a blocked consumer") {
  BlockingQueue<int> q{ 1 };
  std::thread c{ [&] { CHECK_FALSE(q.pop().has_value()); } };
  q.close();
  c.join();
}
