#include <iostream>

class Nombre {
public:
  Nombre( int n ) : n_{ n } {}      // NOT explicit: also converts int -> Nombre
  int value() const { return n_; }
private:
  int n_;
};

void foo( Nombre n ) { std::cout << "foo(Nombre " << n.value() << ")\n"; }

class Price {
public:
  Price( double v ) : v_{ v } {}    // should be explicit!
  double value() const { return v_; }
private:
  double v_;
};
class Quantity {
public:
  Quantity( double q ) : q_{ q } {} // should be explicit!
  double value() const { return q_; }
private:
  double q_;
};

void send_order( Price p, Quantity q ) {
  std::cout << "sending order at " << p.value() << " for " << q.value() << " shares\n";
}

int main() {
  foo( 3 );                         // int converted implicitly
  double px = 100.5, qty = 7;
  send_order( px, qty );            // correct...
  // send_order( qty, px );         // ...but this would compile too, silently!
}
