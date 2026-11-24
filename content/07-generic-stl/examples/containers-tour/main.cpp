#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <list>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
  std::deque<int> d{ 1, 2 };
  d.push_front( 0 );                                   // O(1) at the front too
  std::cout << "deque:"; for( int x : d ) std::cout << " " << x; std::cout << "\n";

  std::list<int> l{ 1, 2, 3 };
  l.insert( std::next( l.begin() ), 99 );              // O(1) given the position
  std::cout << "list after insert:"; for( int x : l ) std::cout << " " << x; std::cout << "\n";

  std::set<std::string> tickers{ "NVDA", "AAPL", "MSFT", "AAPL" };
  std::cout << "set (sorted, unique):"; for( auto & t : tickers ) std::cout << " " << t; std::cout << "\n";

  std::unordered_map<std::string, double> fx{ { "EURUSD", 1.08 }, { "GBPUSD", 1.27 } };
  std::cout << "unordered_map: EURUSD -> " << fx["EURUSD"] << "\n";

  std::stack<int> s;  s.push( 1 ); s.push( 2 ); s.push( 3 );
  std::queue<int> q;  q.push( 1 ); q.push( 2 ); q.push( 3 );
  std::cout << "stack top: " << s.top() << ", queue front: " << q.front() << "\n";

  std::vector<int> v{ 1, 2, 3 };
  std::cout << "reversed:"; for( auto it = v.rbegin(); it != v.rend(); ++it ) std::cout << " " << *it; std::cout << "\n";
  std::vector<int> out;
  std::copy( v.rbegin(), v.rend(), std::back_inserter( out ) );   // writes become push_back
  std::cout << "back_inserter copy:"; for( int x : out ) std::cout << " " << x; std::cout << "\n";
}
