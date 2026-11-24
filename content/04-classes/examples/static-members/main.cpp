#include <iostream>

class A {
public:
   A() { ++nb_instances_; }
  ~A() { --nb_instances_; }
  A( const A & ) { ++nb_instances_; }

  static int nb_instances() {
    return nb_instances_;           // no 'this' in a static method
  }

private:
  inline static int nb_instances_{ 0 };
};

class Order {
public:
  Order() : id_{ ++next_id_ } {}
  long id() const { return id_; }
private:
  long id_;
  inline static long next_id_{ 0 };   // shared by all orders
};

int main() {
  std::cout << A::nb_instances() << "\n";
  A a1, a2;
  std::cout << A::nb_instances() << "\n";
  {
    A a3{ a1 };
    std::cout << A::nb_instances() << "\n";
  }
  A* p = new A;
  delete p;
  std::cout << A::nb_instances() << "\n";   // back to a1, a2
  Order o1, o2, o3;
  std::cout << "orders: " << o1.id() << " " << o2.id() << " " << o3.id() << "\n";
}
