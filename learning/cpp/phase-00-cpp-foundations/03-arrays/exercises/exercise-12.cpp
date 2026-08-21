/*
Exercise 12 — Row and column sums
Difficulty: HARD

Goal:
Two traversal orders over the same data.

Requirements:
- For a flat rows x cols matrix, compute the sum of each row and each column.
- Write each as its own function with a clear signature.
- For a large matrix (at least 2000x2000, heap-allocated -- see note), time both.
- Note: an array this size will not fit on the stack. Use 
ew float[n] and
  delete[] for now; Module 06 explains why.

Questions before coding:
1. Which one walks memory in order? Predict which is faster.
2. By what factor? Predict a number BEFORE measuring.
3. Does the ratio change with matrix size? Predict, then test.

Why this matters for CUDA:
This is the CPU version of coalesced versus strided access. The same experiment on a GPU is fundamentals/02, and the factor there is larger.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}