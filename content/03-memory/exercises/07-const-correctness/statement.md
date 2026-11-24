The functions in `positions.hpp` compute the right values, but their **signatures** are wrong: the tests (which you can't change) pass **const** data and literals, so nothing compiles.

Fix the parameter and return types (`const` placement, pointer vs reference) so that the tests compile and pass. Don't change what the functions compute.
