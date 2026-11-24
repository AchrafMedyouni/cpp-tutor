---
title: "Canonical form, default/delete, initialisation, static members"
slides: "113–119"
---

## The default constructor

A constructor callable **without arguments** (possibly thanks to default values) is the **default constructor**. It's required for `Tableau t;`, for `Tableau t{};`, and for arrays like `Tableau tab[10];`, which default-construct every element.

Slide 113 asks: *where is the intruder?*

```cpp
Tableau t1( 10 );   Tableau t2{ 10 };   Tableau t3();   Tableau t4{};   Tableau t5;
```

`Tableau t3();` **declares a function** named `t3` that takes nothing and returns a `Tableau`. It doesn't create an object. This is the *most vexing parse*, and one more reason to prefer `{}`.

::example{id="vexing-parse"}

## The canonical form of a concrete class

A class that owns a resource needs these four together. If you write one, you almost always need all of them (the *rule of three*, which chapter 7 extends to five with move operations):

```cpp
class Tableau {
public:
  Tableau( unsigned taille = 100 );              // default constructor
  Tableau( const Tableau & tab );                // copy constructor
  ~Tableau();                                    // destructor
  Tableau & operator=( const Tableau & tab );    // copy assignment
};
```

## `= default` and `= delete`

You can explicitly ask for the compiler-generated version (`= default`) or **forbid** an operation (`= delete`). For example, an object with an *identity* (a connection, an order-ID generator) shouldn't be copyable:

```cpp
class X { X( const X & ) = delete; X & operator=( const X & ) = delete; };
class Y { Y( const Y & ) = default; Y & operator=( const Y & ) = default; };
```

::example{id="noncopyable"}

## Initialisation ≠ assignment

A `const` member (like `unsigned int const taille_;`) **can't be assigned in the constructor body**, because by the time the body runs, every member has already been initialised. Members are initialised in the **member initialiser list**:

```cpp
Tableau( unsigned taille ) : taille_{ taille }, tab_{ new double[taille] } { /* Rien */ }
```

Members are initialised in their **declaration order** in the class, whatever the order of the list. Use the list systematically, not only for `const` members: it is the only way to initialise references and members without a default constructor, and it avoids "default-construct, then assign".

::example{id="init-list"}

## Constructors as conversion operators: `explicit`

A constructor that can be called with **one argument** also acts as an **implicit conversion**. If `Nombre(int)` exists, `foo( 3 )` compiles even though `foo` expects a `Nombre`. Sometimes that's handy, but often it's a bug: in finance, a raw `double` silently becoming a `Price`. Mark such constructors **`explicit`** to forbid the implicit conversion.

::example{id="explicit-ctor"}

## Static members (class members)

A `static` attribute belongs to the **class**, not to an instance: there is one copy shared by all objects. A `static` method has no `this`, and is called as `A::nb_instances()`. Since C++17, `inline static` lets you initialise the attribute right in the class.

::example{id="static-members"}
