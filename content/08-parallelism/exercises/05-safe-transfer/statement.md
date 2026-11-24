`transfer(from, to, amount)` locks `from` then `to`. When two threads transfer in **opposite directions** at the same time, each holds one lock and waits for the other: a **deadlock** (the tests time out).

Rewrite `transfer` so that it can't deadlock and stays race-free. A transfer is refused (returns `false`) if `from` has insufficient funds. Transferring to the same account returns `false` and changes nothing.
