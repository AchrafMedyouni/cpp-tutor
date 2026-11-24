#include <iostream>
#include <string>

namespace fr::centralesupelec {
struct Etudiant {
  std::string nom;
  int promo;
};
}  // namespace fr::centralesupelec

// Two "vendors" both providing price(): no clash thanks to namespaces.
namespace vendor_a { double price(double spot) { return spot * 1.01; } }
namespace vendor_b { double price(double spot) { return spot - 0.5; } }

int main() {
  fr::centralesupelec::Etudiant e1{"Ada", 2027};      // 1. qualified access

  {
    using namespace fr::centralesupelec;              // 2. whole namespace (local to this block)
    Etudiant e2{"Alan", 2026};
    std::cout << e2.nom << " " << e2.promo << "\n";
  }

  using fr::centralesupelec::Etudiant;                // 3. a single name
  Etudiant e3{"Grace", 2028};

  namespace cs = fr::centralesupelec;                 // 4. alias
  cs::Etudiant e4{"Edsger", 2025};

  std::cout << e1.nom << ", " << e3.nom << ", " << e4.nom << "\n";
  std::cout << vendor_a::price(100) << " vs " << vendor_b::price(100) << "\n";
}
