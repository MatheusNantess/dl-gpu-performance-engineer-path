/*
Exercise 09 — Float precision
Difficulty: MEDIUM

Goal:
Observe floating-point error accumulating, on a scale small enough to follow.

Requirements:
- Add 0.1f to a running float sum 10 times, then 1000, then 1000000 times.
- Print the result and the absolute error against the exact expected value.
- Repeat the whole thing with double.
- Print with enough digits to see the difference (std::setprecision, <iomanip>).
- Then: sum 1.0f a million times, and separately sum a large value and many
  tiny values. Compare.

Questions before coding:
1. Why is 0.1 not exactly representable in binary floating point?
2. Does the error grow linearly with the number of additions, or faster?
3. If you sum the same numbers in a different ORDER, do you get the same result?

Why this matters for CUDA:
A reduction kernel sums millions of floats. Summation order changes the
result, GPU reductions sum in a different order than a sequential CPU loop,
and this is exactly why "correct" needs a tolerance rather than equality.
*/

#include <iostream>
#include <iomanip>

int main() {
    // TODO: implement
    return 0;
}