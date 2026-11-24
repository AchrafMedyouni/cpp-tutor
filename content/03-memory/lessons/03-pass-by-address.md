---
title: "Passing and returning by address"
slides: "63–64"
---

To let a function **modify the caller's variable**, pass its address:

```cpp
void f2( int * i ) { int k{ *i }; *i = 3; }
f2( &j );          // ✓
// f2( &5 );       // ✗ a literal has no address
// f2( j );        // ✗ an int is not an int*
// f2( &( j + 2 ));// ✗ a temporary result has no address
```

The slides call the name **abusive**, because this is really *pass-by-value of a pointer*: the function gets a copy of the address, and through that copy it reaches the original variable.

::example{id="pass-by-address"}


A function can also **return an address**, but the object pointed to **must still exist after the function returns**. A `static` local does: it lives for the whole program. An ordinary local variable doesn't. Returning its address hands the caller a **dangling pointer**.

::example{id="return-address"}

::example{id="return-local-address"}
