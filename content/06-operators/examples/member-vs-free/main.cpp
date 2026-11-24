class Money {
public:
  Money( long cents ) : cents_{ cents } {}       // converting constructor
  Money operator+( const Money & m ) const { return { cents_ + m.cents_ }; }
private:
  long cents_;
};

int main() {
  Money fee{ 250 };
  Money a = fee + 100;     // OK: fee.operator+( Money{100} )
  Money b = 100 + fee;     // error: int has no member operator+ taking Money
  (void)a; (void)b;
}
