A `Desk` owns its traders (`std::vector<std::shared_ptr<Trader>>`), and each `Trader` points back to their desk. With a `shared_ptr` back-pointer this creates a **cycle**: nothing is ever destroyed (slide 178).

Fix `desk.hpp` so that:
- destroying the last external `shared_ptr` to a desk destroys the desk **and** its traders;
- `Trader::desk_name()` returns the desk's name, or `"(none)"` once the desk is gone.
