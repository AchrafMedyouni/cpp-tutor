template< typename T >
class ptr {
public:
  ptr( T * p ) : p_{ p } {}
  ~ptr() { delete p_; }
  T * operator ->() { return p_; }
private:
  T * p_;
};

struct Order { int id; };

int main() {
  ptr< Order > a{ new Order{ 1 } };
  ptr< Order > b{ a };      // default copy: both point to the same Order
}                           // both destructors delete it
