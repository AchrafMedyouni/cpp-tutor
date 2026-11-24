The input is a list of orders, one per line: `SIDE QTY PRICE`, where `SIDE` is `BUY` or `SELL`, `QTY` is a positive integer and `PRICE` a decimal number.

Process them in order and track:
- `position`: the sum of bought quantities minus sold quantities;
- `cash`: decreases by `qty * price` on a BUY and increases by `qty * price` on a SELL.

If an order is malformed (unknown side, or quantity/price not a number), print `error: bad order #k` (k counts from 1), **stop reading**, and still print the summary of the orders processed so far. At the end print:

```
position: 50
cash: -645.00
```
`cash` has exactly 2 decimals.
