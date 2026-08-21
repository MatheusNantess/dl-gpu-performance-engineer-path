/*
Challenge 01 — Array scaler

Combines: pointers, functions, arrays

Goal:
Implement the simplest real kernel shape, and every variation of it.

Requirements:
- Implement: void scale(float* data, int n, float factor)
- Then an out-of-place version: void scale_out(const float* in, float* out, int n, float factor)
- Then one using pointer arithmetic only, no [] anywhere.
- Handle n == 0 and a null pointer.
- Verify all three agree on the same input.

Questions before coding:
1. Which version needs const, and on which parameter?
2. How many bytes does the in-place version move? The out-of-place one?
3. Which of the two would be faster on a memory-bound machine, and why?

Byte accounting:
State, in a comment with your solution, how many bytes this reads and how
many it writes, for n elements. This is the habit that becomes roofline
analysis later.

Why this matters for CUDA:
scale() is one of the first CUDA kernels in this lab. The signature is identical; only the loop becomes a thread index.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}