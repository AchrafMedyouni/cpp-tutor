#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

std::condition_variable box_empty, box_full;
std::mutex data_mutex;
bool data_in_box{ false };
int box;                                // the one-slot mailbox
constexpr int N = 10;

struct Producer {
  void operator()() {
    for( int i = 1; i <= N; ++i ) {     // while( !done() ) produce_data();
      std::unique_lock< std::mutex > lock{ data_mutex };
      while( data_in_box ) {            // the slide uses `if`
        box_empty.wait( lock );
      }
      box = i;                          // put_data_into_box();
      data_in_box = true;
      box_full.notify_one();
    }
  }
};

struct Consumer {
  std::vector<int> * received;
  void operator()() {
    for( int k = 0; k < N; ++k ) {
      int value;
      {
        std::unique_lock< std::mutex > lock{ data_mutex };
        while( ! data_in_box ) {
          box_full.wait( lock );
        }
        value = box;                    // get_data_from_box();
        data_in_box = false;
        box_empty.notify_one();
      }
      received->push_back( value );     // consume_data(), outside the lock
    }
  }
};

int main() {
  std::vector<int> received;
  std::thread c{ Consumer{ &received } };
  std::thread p{ Producer() };
  c.join();
  p.join();
  int sum = 0;
  std::cout << "consumed";
  for( int v : received ) { std::cout << " " << v; sum += v; }
  std::cout << "\nsum = " << sum << "\n";
}
