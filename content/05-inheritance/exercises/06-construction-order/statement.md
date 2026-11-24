`Instrument` has **no default constructor**, so `Bond` must call `Instrument(std::string)` from its initialiser list (slide 125). Write `Bond(std::string name, double coupon)` with `double coupon() const`.

Every constructor and destructor appends a line to the shared `trace()` log: `"Instrument(<name>)"`, `"Bond"`, `"~Bond"`, `"~Instrument"`. The tests check the **order**: the base is constructed first and destroyed last.
