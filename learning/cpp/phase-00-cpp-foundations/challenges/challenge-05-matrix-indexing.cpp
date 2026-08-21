/*
Challenge 05 — Matrix indexing

Combines: row-major layout, 2D indexing, traversal order

Goal:
Full fluency with flat 2D indexing.

Requirements:
- Represent a rows x cols matrix as a flat float array on the heap.
- Implement: get, set, row sum, column sum, transpose, and matrix multiply.
- For matmul, use the naive triple loop.
- Verify against hand-computed 2x3 and 3x2 examples.
- Then time the matmul with the inner loops in two different orders and compare.

Questions before coding:
1. In the matmul triple loop, which loop order gives contiguous access for each
   of the three matrices? Can all three be contiguous at once?
2. How many times is each element of A read? Of B?
3. What is the arithmetic intensity of matmul, and how does it change with size?

Byte accounting:
State, in a comment with your solution, how many bytes this reads and how
many it writes, for n elements. This is the habit that becomes roofline
analysis later.

Why this matters for CUDA:
GEMM is kernel 08 in this repository. The loop-order question here is the one that leads to tiling, which is the first serious GPU optimization.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}