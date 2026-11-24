`EquityFeed` derives from `MarketFeed` and owns a heap buffer. The tests create feeds through a factory returning `MarketFeed*` and `delete` them through that base pointer.

Right now the derived destructor **never runs**: the buffer leaks and the counter of closed feeds stays at 0. Fix the base class.
