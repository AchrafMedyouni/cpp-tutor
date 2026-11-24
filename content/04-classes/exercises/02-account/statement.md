Write `class Account` whose **invariant** is `balance() >= 0`:

- `explicit Account(std::string owner, double initial = 0)`: throws `std::invalid_argument` if `initial < 0`
- `const std::string& owner() const`, `double balance() const`
- `void deposit(double amount)`: throws `std::invalid_argument` if `amount <= 0`
- `bool withdraw(double amount)`: `false` (and no change) if `amount <= 0` or `amount > balance()`
- `int operations() const`: the number of **successful** deposits and withdrawals
