#include <chrono>
#include <iostream>
#include <latch>
#include <mutex>
#include <string>
#include <thread>

using namespace std::chrono_literals;

std::timed_mutex A, B;
std::latch both_hold_first_lock{ 2 };   // C++20: makes the bad interleaving certain
std::latch both_tried{ 2 };             // keep the first lock until both threads have tried
std::string report[2];

void worker( int id, std::timed_mutex & first, std::timed_mutex & second, const char * names ) {
  std::lock_guard<std::timed_mutex> l1{ first };
  both_hold_first_lock.arrive_and_wait();            // wait until the other thread holds ITS first lock
  std::unique_lock<std::timed_mutex> l2{ second, std::defer_lock };
  bool ok = l2.try_lock_for( 200ms );                 // a plain lock() would block forever here
  report[id] = std::string{ "t" } + char( '1' + id ) + " holds " + names[0] + ", wants " + names[1] +
               ( ok ? ": got it" : ": gave up after timeout" );
  both_tried.arrive_and_wait();                      // still holding `first`
}

int main() {
  std::thread t1{ worker, 0, std::ref( A ), std::ref( B ), "AB" };   // A then B
  std::thread t2{ worker, 1, std::ref( B ), std::ref( A ), "BA" };   // B then A: opposite order
  t1.join();
  t2.join();
  std::cout << report[0] << "\n" << report[1] << "\n";
  std::cout << "both threads waited for each other: deadlock (circular wait)\n";
}
