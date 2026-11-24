Turn the aggregate into a class (slides 86–97), split across a header and an implementation file:

- private attributes `real_`, `imag_`;
- constructors `Complexe()` (zero) and `Complexe(double re, double im)`;
- accessors `double real() const`, `void real(double)`, `double imag() const`, `void imag(double)`;
- `double module() const` (defined in `Complexe.cpp`) and `void ajoute(const Complexe& other)`.
