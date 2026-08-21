/*
Exercise 06 — Locality measurement
Difficulty: HARD

Goal:
Measure the cache effect directly. The most important experiment here.

Requirements:
- Allocate a large float array on the heap (start at 64 MB).
- Sum it three ways, timing each: sequential, stride 16, and random order.
- Warm up, repeat, report medians.
- Then repeat the whole experiment with an array small enough to fit in cache
  (a few hundred KB) and compare the ratios.
- Record all results in checkpoint.md.

Questions before coding:
1. Predict the three ratios BEFORE measuring. Write them down.
2. All three versions perform the same number of additions. What costs the difference?
3. Why do the ratios shrink when the array fits in cache?

Why this matters for CUDA:
This is the CPU cousin of memory coalescing and of the roofline model, both central to this repository. The same experiment on a GPU is fundamentals/02.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}