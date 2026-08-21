/*
Exercise 07 — Predict the addresses
Difficulty: MEDIUM

Goal:
Reason about addresses arithmetically, not by running.

Requirements:
- Declare arrays of char, int, float and double, each with 5 elements.
- BEFORE compiling, predict the byte gap between consecutive elements of each.
- Also predict: given the address of element 0, what is the address of element 3?
- Then print the actual addresses and gaps and compare.
- Cast to const char* to compute byte differences.

Questions before coding:
1. Write the general formula for the address of element i.
2. Are the four arrays adjacent to each other in memory? Predict, then check.
3. Why is the absolute address unpredictable but the gap not?

Why this matters for CUDA:
This formula is what a GPU thread computes to find its data. Whether 32 threads land on contiguous addresses is the coalescing question.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}