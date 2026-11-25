#include <iostream>

struct Position { long qty; double price; };

const Position* caller = nullptr;   // remembers main's object, to tell "same object" from "a copy"
const char* which(const Position& p) { return &p == caller ? "pos itself" : "a different object"; }

void byValue(Position p) {          // gets its own copy
  p.qty = 0;
  std::cout << "byValue:    modified " << which(p) << '\n';
}
void byRef(Position& p) {           // an alias of the caller's object
  p.qty = 0;
  std::cout << "byRef:      modified " << which(p) << '\n';
}
void byConstRef(const Position& p) {  // an alias, read-only: no copy, no modification
  std::cout << "byConstRef: read " << which(p) << ", notional " << p.qty * p.price << '\n';
}
void byPtr(Position* p) {           // an address: may be nullptr ("no object")
  if (p == nullptr) {
    std::cout << "byPtr:      got nullptr, nothing to do\n";
    return;
  }
  p->qty = 0;
  std::cout << "byPtr:      modified " << which(*p) << '\n';
}

int main() {
  Position pos{ 100, 2.5 };
  caller = &pos;

  byValue(pos);
  std::cout << "  -> pos.qty = " << pos.qty << "  (unchanged: the function changed its copy)\n";
  byConstRef(pos);
  byRef(pos);
  std::cout << "  -> pos.qty = " << pos.qty << '\n';

  pos.qty = 100;
  byPtr(&pos);                      // the & is visible at the call site
  std::cout << "  -> pos.qty = " << pos.qty << '\n';
  byPtr(nullptr);

  byConstRef(Position{ 4, 2.5 });   // a temporary binds to const&
  // byRef(Position{ 4, 2.5 });     // ✗ but not to a non-const &
}
