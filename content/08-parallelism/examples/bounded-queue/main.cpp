#include <cmath>
#include <condition_variable>
#include <deque>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <optional>
#include <thread>

template <typename T>
class BoundedQueue {
public:
  explicit BoundedQueue( std::size_t cap ) : cap_{ cap } {}
  void push( T v ) {
    std::unique_lock<std::mutex> lock{ m_ };
    not_full_.wait( lock, [&] { return q_.size() < cap_; } );   // predicate form: loops for us
    q_.push_back( std::move( v ) );
    not_empty_.notify_one();
  }
  std::optional<T> pop() {                                       // empty optional = closed and drained
    std::unique_lock<std::mutex> lock{ m_ };
    not_empty_.wait( lock, [&] { return !q_.empty() || closed_; } );
    if( q_.empty() ) return std::nullopt;
    T v = std::move( q_.front() );
    q_.pop_front();
    not_full_.notify_one();
    return v;
  }
  void close() {
    std::lock_guard<std::mutex> lock{ m_ };
    closed_ = true;
    not_empty_.notify_all();
  }
private:
  std::mutex m_;
  std::condition_variable not_full_, not_empty_;
  std::deque<T> q_;
  std::size_t cap_;
  bool closed_ = false;
};

struct Tick { int seq; double price; };

int main() {
  BoundedQueue<Tick> feed{ 16 };
  std::jthread producer{ [&] {
    for( int i = 0; i < 200; ++i )                               // deterministic synthetic prices
      feed.push( { i, 100.0 + 2.0 * std::sin( i / 7.0 ) + ( i % 3 ) * 0.1 } );
    feed.close();
  } };

  int consumed = 0, buys = 0, sells = 0, state = 0;
  double window[5] = {}, sma = 0;
  while( auto t = feed.pop() ) {                                 // strategy: price vs SMA(5) crossover
    window[consumed % 5] = t->price;
    ++consumed;
    if( consumed < 5 ) continue;
    sma = ( window[0] + window[1] + window[2] + window[3] + window[4] ) / 5;
    int s = t->price > sma ? 1 : -1;
    if( s != state && state != 0 ) ( s > 0 ? buys : sells )++;
    state = s;
  }
  std::cout << "ticks consumed: " << consumed << "\n";
  std::cout << "signals: " << buys << " BUY, " << sells << " SELL\n";
  std::cout << std::fixed << std::setprecision(2) << "last SMA(5) = " << sma << "\n";
}
