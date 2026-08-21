/*
Exercise 06 — Multiple outputs
Difficulty: MEDIUM

Goal:
Return more than one value, without containers.

Requirements:
- Write min_max(const int* data, int n, int* out_min, int* out_max) that finds
  both in a single pass.
- Write mean_and_count that returns the mean through a pointer and the count
  as the return value.
- Do not use std::pair, std::tuple, std::vector, or any STL algorithm.
- Handle n == 0 sensibly and document what you chose.

Questions before coding:
1. Why const on the input pointer and not on the output pointers?
2. What should happen if out_min is nullptr? Decide and document.
3. Could you do this in one pass or does it need two? Why?

Why this matters for CUDA:
A kernel computing several reductions writes each to a separate output pointer.
This is exactly that pattern, one thread's worth.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}