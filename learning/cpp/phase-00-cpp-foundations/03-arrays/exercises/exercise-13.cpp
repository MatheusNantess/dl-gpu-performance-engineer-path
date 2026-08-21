/*
Exercise 13 — Sliding window
Difficulty: HARD

Goal:
Boundary handling, which is where most array bugs live.

Requirements:
- Compute a moving average with window size k over a float array.
- Handle the boundaries explicitly -- decide and document what happens at the
  first and last k/2 elements.
- Do it without allocating a second array, then explain why that is harder
  than it looks.
- Verify against a hand-computed result on a small input.

Questions before coding:
1. Why does the in-place version give wrong answers if written naively?
2. How many elements does each output touch? How many total reads for n outputs?
3. Is this compute-bound or memory-bound? Estimate the arithmetic intensity.

Why this matters for CUDA:
Overlapping reads are the shape of convolution and of stencil kernels, where the same input element is read by several threads -- which is what makes shared memory worthwhile.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}