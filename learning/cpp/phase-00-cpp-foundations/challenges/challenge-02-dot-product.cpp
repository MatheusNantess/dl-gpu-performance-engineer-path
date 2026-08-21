/*
Challenge 02 — Dot product

Combines: pointers, const, numerics, reductions

Goal:
A reduction over two arrays, with the accuracy question attached.

Requirements:
- Implement: float dot_product(const float* a, const float* b, int n)
- Then a version accumulating into a double.
- Compare both against a known result on at least one million elements.
- Then implement pairwise (divide-and-conquer) summation and compare all three.
- Measure the runtime of each.

Questions before coding:
1. Why do the three versions disagree? Which is most accurate?
2. How many bytes are read per element? How many written?
3. Is this compute-bound or memory-bound? Compute the arithmetic intensity.

Byte accounting:
State, in a comment with your solution, how many bytes this reads and how
many it writes, for n elements. This is the habit that becomes roofline
analysis later.

Why this matters for CUDA:
The dot product is the core of GEMM and attention. Accumulator precision and summation order are live design decisions in mixed-precision kernels -- and GPU reductions sum in a different order than a sequential loop, which is why correctness needs a tolerance.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}