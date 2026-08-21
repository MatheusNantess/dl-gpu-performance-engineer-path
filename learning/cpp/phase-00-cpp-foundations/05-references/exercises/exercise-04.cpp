/*
Exercise 04 — const reference and copy cost
Difficulty: MEDIUM

Goal:
Measure the cost of copying something large.

Requirements:
- Define a struct with a large array member (at least 4 KB).
- Write functions taking it by value and by const reference.
- Call each many times in a loop and time both with <chrono>.
- Use warmup and report the median of several repetitions.
- Print sizeof(struct) and compute total bytes copied in the by-value version.

Questions before coding:
1. Predict the ratio BEFORE measuring.
2. How many bytes does the by-value version copy in total across the loop?
3. At what struct size does the difference become measurable?

Why this matters for CUDA:
The GPU version of this mistake is copying a tensor to the device unnecessarily, or writing an intermediate to global memory that could have stayed in registers.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}