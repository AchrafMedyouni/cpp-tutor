#include <iostream>
#include <memory>

struct Forme { virtual ~Forme() = default; };
struct Cercle : Forme {
  int r;
  explicit Cercle( int r ) : r{ r } {}
  ~Cercle() override { std::cout << "~Cercle(" << r << ")\n"; }
};

int main() {
  std::shared_ptr< int > p1{ new int{ 1 }};
  auto p2{ std::make_shared< int >( 2 )};      // preferred
  std::cout << "p2 use_count = " << p2.use_count() << "\n";
  {
    auto p3 = p2;                              // shares ownership
    std::cout << "after copy: " << p2.use_count() << "\n";
  }
  std::cout << "after scope: " << p2.use_count() << "\n";

  std::shared_ptr< Forme > f1{ new Cercle{ 1 }};
  auto c1{ std::make_shared< Cercle >( 5 )};
  f1 = c1;                                     // Cercle(1) count drops to 0: deleted now
  std::cout << "f1 = c1: c1.use_count() = " << c1.use_count() << "\n";
}                                              // Cercle(5) deleted when the last owner goes
