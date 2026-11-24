`struct Complexe { double real; double imag; };` is given. Write:

- `double module(Complexe c)`
- `Complexe conjugate(Complexe c)`
- `Complexe add(Complexe a, Complexe b)`
- `bool approx_equal(Complexe a, Complexe b, double eps = 1e-9)`: true when both parts differ by at most `eps`. (Remember: `==` doesn't exist for a struct, and comparing doubles exactly is fragile anyway.)
