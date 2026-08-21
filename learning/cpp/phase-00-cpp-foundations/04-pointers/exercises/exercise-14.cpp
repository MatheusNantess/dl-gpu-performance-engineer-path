/*
Exercise 14 — const correctness
Difficulty: MEDIUM

Goal:
Find out what each const placement forbids.

Requirements:
- Write functions taking: float*, const float*, float* const, const float* const.
- In each, attempt to (a) modify the pointed-to value and (b) reassign the pointer.
- Comment out what does not compile and record each exact error message.
- Build a table in a comment: which of the four allow which operation.

Questions before coding:
1. Which placement is used in kernel signatures for inputs?
2. Read const float* const p right to left. What does it say?
3. Does const change what the generated code does, or only what compiles?

Why this matters for CUDA:
Kernel inputs are const float*, outputs are float*. That distinction enables compiler optimizations and, on some GPUs, a faster read-only cache path.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}