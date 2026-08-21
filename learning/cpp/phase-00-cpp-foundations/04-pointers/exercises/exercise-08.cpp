/*
Exercise 08 — Pointer output parameters
Difficulty: MEDIUM

Goal:
Return several values without containers.

Requirements:
- Write void stats(const float* data, int n, float* out_mean, float* out_min, float* out_max).
- Compute all three in a single pass.
- Call it and print the results.
- Then make one of the output pointers optional: if it is nullptr, skip that output.
- Do not use std::tuple, std::pair or any container.

Questions before coding:
1. Why are the outputs pointers and the input const?
2. How do you check for nullptr before writing?
3. Could the input and an output ever be the same memory? What would happen?

Why this matters for CUDA:
A kernel computing several reductions writes each to a separate output pointer. This is that pattern.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}