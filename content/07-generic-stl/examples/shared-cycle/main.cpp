#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Desk;

struct Trader {
  std::string name;
  std::shared_ptr<Desk> desk;              // back-reference that OWNS: creates a cycle
  ~Trader() { std::cout << "~Trader " << name << "\n"; }
  std::string desk_name() const;
};

struct Desk {
  std::string name;
  std::vector< std::shared_ptr<Trader> > traders;
  ~Desk() { std::cout << "~Desk " << name << "\n"; }
};

std::string Trader::desk_name() const { return desk ? desk->name : "(none)"; }

int main() {
  auto rates = std::make_shared<Desk>( Desk{ "Rates", {} } );
  auto alice = std::make_shared<Trader>( Trader{ "Alice", nullptr } );
  rates->traders.push_back( alice );
  alice->desk = rates;                     // Desk → Trader → Desk
  std::cout << alice->name << " works on " << alice->desk_name() << "\n";
}   // counts never reach 0: no destructor runs, both objects leak
