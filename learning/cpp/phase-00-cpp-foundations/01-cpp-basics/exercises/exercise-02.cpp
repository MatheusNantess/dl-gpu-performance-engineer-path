/*
Exercise 02 — Memory footprint calculator
Difficulty: EASY

Goal:
Turn element counts into bytes, the calculation done constantly in kernel work.

Requirements:
- Read an element count n from the user (std::cin).
- Print the memory required for n elements as float, double, and char.
- Print each in bytes, KB and MB.
- Use size_t for the element count. Consider why.

Questions before coding:
1. If n is 100 million and you use int for the byte total, what happens?
2. Why is the float total exactly half the double total?
3. A 1024x1024 FP32 matrix -- how many MB? Compute it by hand first.

Why this matters for CUDA:
An RTX 3060 has 12 GB. Knowing whether a tensor fits is this arithmetic.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}