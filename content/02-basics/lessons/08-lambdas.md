---
title: "Lambdas"
slides: "51–54"
---

C++ doesn't let you define a named function inside another function. You often also want a small piece of behaviour without naming it at all. **Lambda expressions** cover both needs. They come from functional languages, and Python has a restricted version (`lambda x: ...`).

```cpp
[]( int n ) -> bool { return n % 2 == 0; }
```

- `[]` introduces the lambda and lists its **captures**: which variables from the surrounding scope it may use, and how.
- `( int n )` is the parameter list.
- `-> bool` is the return type. It is optional when the compiler can deduce it from the `return` statements.
- `{ ... }` is the body. Unlike Python's lambda, it can contain any statements.

A lambda's type is unique and unnamed, so store it with `auto`, or in a `std::function` when you need a uniform type (e.g. a container of callbacks). You can also pass it straight to an algorithm:

::example{id="lambda-basics"}

:::course
Slide 53 writes `std::fonction< int( bool ) > f2{ f1 };`. The template is `std::function`, and the lambda takes an `int` and returns a `bool`, so the type is `std::function< bool( int ) >`.
:::

## Capturing the context

A lambda can use variables of the enclosing function if it **captures** them:
- `[i]` captures **by value**: a copy taken *when the lambda is created*;
- `[&j]` captures **by reference**: the lambda sees later changes to `j`, and can modify it;
- `[=]` / `[&]` capture everything that is used, by value or by reference respectively.

::example{id="lambda-capture"}

::example{id="payoff-lambdas"}

Capturing by reference has a trap: the lambda must not outlive the variable it refers to.

::example{id="dangling-capture"}
