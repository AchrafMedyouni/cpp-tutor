#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Instrument {
  virtual ~Instrument() = default;
  virtual std::string name() const = 0;
  virtual double price() const = 0;
};
struct Bond : Instrument { std::string name() const override { return "bond"; } double price() const override { return 98.5; } };
struct Option : Instrument { std::string name() const override { return "option"; } double price() const override { return 7.25; } };
struct Future : Instrument { std::string name() const override { return "future"; } double price() const override { return 101.2; } };

std::unique_ptr<Instrument> make( const std::string & kind ) {   // a factory returns ownership
  if( kind == "bond" ) return std::make_unique<Bond>();
  if( kind == "option" ) return std::make_unique<Option>();
  return std::make_unique<Future>();
}

void take_ownership( std::unique_ptr<Instrument> p ) {            // by value: the caller must move
  std::cout << "owner prices it at " << p->price() << "\n";
}                                                                  // deleted here

int main() {
  std::vector< std::unique_ptr<Instrument> > book;
  for( auto k : { "bond", "option", "future" } ) book.push_back( make( k ) );
  std::cout << "book holds " << book.size() << " instruments\n" << std::fixed << std::setprecision(2);
  for( const auto & i : book ) std::cout << i->name() << ": " << i->price() << "\n";

  auto opt = std::move( book[1] );                                 // transfer
  std::cout << "moved out the option: book[1] is " << ( book[1] ? "still set" : "null now" ) << "\n";
  take_ownership( std::move( opt ) );
}   // the remaining instruments are deleted by the vector's unique_ptrs
