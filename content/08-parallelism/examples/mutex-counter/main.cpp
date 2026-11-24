#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

class ProtectedInt {
public:
  ProtectedInt( int n ) : n_( n ) {}
  void operator+=( int i ) {
    std::lock_guard< std::mutex > l( mutex_ );   // locked here...
    n_ += i;
  }                                              // ...unlocked here, even on exception
  int value() {
    std::lock_guard< std::mutex > l( mutex_ );
    return n_;
  }
private:
  int n_;
  std::mutex mutex_;
};

void f( ProtectedInt & x ) {
  for( int i = 0; i < 1'000'000; ++i ) x += 1;
}

int main() {
  ProtectedInt n{ 2 };
  std::thread t1{ f, std::ref( n ) };
  std::thread t2{ f, std::ref( n ) };
  t1.join(); t2.join();
  std::cout << "n = " << n.value() << "\n";
}
