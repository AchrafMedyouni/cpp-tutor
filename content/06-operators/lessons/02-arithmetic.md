---
title: "Arithmetic and assignment operators"
slides: "141–142"
---

Two styles (slide 141):

```cpp
class Complexe {
public:
  Complexe operator+( const Complexe & c );          // object approach: c1 + c2 → c1.operator+( c2 )
  friend Complexe operator*( const Complexe & c1,    // functional approach: c1 * c2 → operator*( c1, c2 )
                             const Complexe & c2 );
};
```

- A **member** operator's left operand must already be a `Complexe`. Implicit conversions apply only to the right operand, so with a converting constructor `Complexe(double)`, `c + 2.0` works but `2.0 + c` doesn't.
- A **free** operator treats both operands symmetrically. **`friend`** grants it access to the private members.
- Assignment operators (`=`, `+=` …) **must be members** (the *object approach is mandatory*) and conventionally return `*this` by reference.

The idiomatic recipe from slide 142: write **`+=` as a member**, then **`+` as a free function built on it**.

```cpp
Complexe operator+( const Complexe & c1, const Complexe & c2 ) {
  Complexe res{ c1 };
  return res += c2;
}
```

::example{id="complexe-operators"}

::example{id="member-vs-free"}

:::modern
Make an operator a member when it modifies the left operand (`=`, `+=`, `[]`, `()`, `++`). Make it a free function when it is symmetric (`+`, `*`, `==`, `<<`). `const` matters too: the member `operator+` on the slide should be `const` (`Complexe operator+( const Complexe & c ) const;`), otherwise it can't be called on a const left operand.
:::
