/*
Exercise 17 — Predict: 2D through pointers
Difficulty: HARD

Goal:
Combine row-major layout with pointer arithmetic.

Requirements:
- Given a flat float array representing a 4x6 matrix:
- Predict, in writing, the flat index and byte offset of (2,3) and (3,5).
- Predict what m + 2*6 points at.
- Write row-pointer code: float* row = m + r * cols; then use row[c].
- Verify all predictions.
- Then write the same access without any [] at all.

Questions before coding:
1. What is the address of the start of row r?
2. What is the stride between (r,c) and (r+1,c)? Between (r,c) and (r,c+1)?
3. Which of those two strides do you want a warp of threads to walk along?

Why this matters for CUDA:
This decomposition -- a pointer to the start of a row -- is exactly how tiled matrix kernels are written.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}