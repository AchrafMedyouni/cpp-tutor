Read prices (decimal numbers separated by spaces or newlines) until the end of the input. Print, with **exactly two decimals**:

```
count: 4
total: 402.50
mean: 100.62
min: 99.00
max: 102.00
```

`count` is an integer, everything else uses 2 decimals: `std::cout << std::fixed << std::setprecision(2)` (from `<iomanip>`). Note that 402.50 / 4 = 100.625 prints as `100.62`: an exact half is rounded to the even digit. If there is no price at all, print just `no data`.
