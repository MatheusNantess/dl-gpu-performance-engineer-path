/*
Exercise 08 — Dot product
Difficulty: MEDIUM

Goal:
Two arrays, one reduction.

Requirements:
- Write float dot(const float* a, const float* b, int n).
- Test with vectors whose answer you can verify by hand.
- Then compute it again accumulating into a double instead of a float.
  Compare, using an array of at least 100000 elements.

Questions before coding:
1. How many bytes are read per element? How many written?
2. Why might the float and double accumulator versions differ?
3. Which one is more accurate, and is it also slower here?

Why this matters for CUDA:
The dot product is the core of GEMM and of attention. The accumulator precision question is a live topic in mixed-precision kernel design.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}