/*
Exercise 01 — Sum, max, min
Difficulty: EASY

Goal:
Basic traversal, three times.

Requirements:
- Declare a fixed int array with at least 10 values.
- Compute the sum, the maximum and the minimum in separate loops.
- Then compute all three in a SINGLE pass.
- Do not use std::vector or any STL algorithm.

Questions before coding:
1. How many times does the multi-loop version read each element? The single-pass version?
2. For a large array, which is faster, and is the reason arithmetic or memory?
3. What should max and min be for an empty array?

Why this matters for CUDA:
These three are the first reduction kernels. The single-pass version is the shape a GPU reduction takes.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}