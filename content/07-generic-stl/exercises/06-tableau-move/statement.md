`Tableau` already has the copy operations (v4). Add the **move constructor** and **move assignment** (slides 172–173):

- moving must **not allocate** and must **not copy** the elements
- the moved-from object must be left **empty but valid** (`taille() == 0`, and safe to destroy or assign to)
- both must be `noexcept`, so that `std::vector<Tableau>` moves rather than copies when it grows
