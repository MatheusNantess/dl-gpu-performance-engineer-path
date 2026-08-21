/*
Exercise 08 — Overflow boundaries
Difficulty: MEDIUM

Goal:
See exactly where fixed-width integers break.

Requirements:
- Start from INT_MAX (<climits>) and add 1. Print before and after.
- Do the same with unsigned int at its maximum.
- Do the same starting from INT_MIN and subtracting 1.
- Compile at -O0 and again at -O2. Compare the output.
- Show a case where long long succeeds and int fails.

Questions before coding:
1. Predict every value before running.
2. Which of these is undefined behaviour and which is defined wraparound?
3. Why might -O0 and -O2 disagree? What does that tell you about UB?

Why this matters for CUDA:
Indexing a tensor with more than 2.1 billion elements using int overflows
and writes to memory the kernel does not own. Production kernels use 64-bit
indices for exactly this reason.
*/

#include <iostream>
#include <climits>

int main() {
    // TODO: implement
    return 0;
}