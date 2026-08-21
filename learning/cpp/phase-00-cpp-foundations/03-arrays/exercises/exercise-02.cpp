/*
Exercise 02 — Average and scaling
Difficulty: EASY

Goal:
Read-only traversal, then in-place modification.

Requirements:
- Compute the average of a float array.
- Then scale every element in place by a factor.
- Print the array before and after.
- Recompute the average and verify it scaled by the same factor.

Questions before coding:
1. Does the scaled average exactly equal average * factor in floating point? Predict, then check.
2. Does scaling read and write, or only write?
3. How many bytes does the scaling pass move, for n elements?

Why this matters for CUDA:
scale() is the canonical first CUDA kernel after vector add. This is its CPU form.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}