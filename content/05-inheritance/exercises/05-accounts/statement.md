Implement the *template method* pattern. The base class fixes the algorithm, and derived classes customise one step through a virtual function.

- `Account(double initial)`: `double balance() const`, and `bool withdraw(double amount)`, which is **non-virtual**. It asks the virtual `bool can_withdraw(double amount) const` first, and only then debits.
- `Checking(double initial, double overdraft)`: may go down to `-overdraft`
- `Savings(double initial)`: may not go negative, and allows at most **3** successful withdrawals
