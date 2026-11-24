#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

void f( std::atomic_int & x ) {
  for( int i = 0; i < 1'000'000; ++i ) x += 1;   // one indivisible read-modify-write
}

int main() {
  std::atomic_int n{ 2 };
  auto start = std::chrono::steady_clock::now();
  std::thread t1{ f, std::ref( n ) };
  std::thread t2{ f, std::ref( n ) };
  t1.join();
  t2.join();
  auto ms = std::chrono::duration<double, std::milli>( std::chrono::steady_clock::now() - start ).count();
  std::cout << "atomic total = " << n << "\n";
  std::cerr << "(took " << ms << " ms)\n";
}
