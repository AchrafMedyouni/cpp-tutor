Now an incoming limit order **matches** against the opposite side before resting:

- a buy at `P` trades with asks priced `≤ P`, best (lowest) price first; a sell at `P` trades with bids priced `≥ P`, highest first
- within a price level, the **oldest** order trades first (time priority)
- each fill trades at the **resting** order's price, and partial fills are allowed
- whatever remains of the incoming order rests in the book at its limit price
- `add_limit` returns the trades in execution order: `Trade{ buy_id, sell_id, price, qty }`
