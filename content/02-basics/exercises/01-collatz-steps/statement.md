Write `int collatz_steps(long long n)`. It returns the number of steps needed to reach 1 when you repeatedly apply: *if n is even, n = n / 2; otherwise n = 3n + 1* (slide 40).

`collatz_steps(1) == 0`, `collatz_steps(6) == 8` (6 → 3 → 10 → 5 → 16 → 8 → 4 → 2 → 1).

The function lives in a header, so define it `inline`.
