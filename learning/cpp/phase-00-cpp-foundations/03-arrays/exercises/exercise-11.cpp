/*
Exercise 11 — Matrix transpose
Difficulty: MEDIUM

Goal:
Write into a different layout, and notice the access pattern.

Requirements:
- Given a flat rows x cols matrix, produce its transpose in a second array.
- Print both.
- Verify that transposing twice returns the original.
- Then note, in a comment: in your loop, is the READ contiguous, the WRITE
  contiguous, or both? Can both be, at once?

Questions before coding:
1. What is the index formula for the output element?
2. If you traverse the input contiguously, what does the output access look like?
3. Could you swap the loops to fix it? What breaks instead?

Why this matters for CUDA:
Matrix transpose is fundamentals/03 in this repository, and its whole point is that ONE side must be non-contiguous. That is what forces the use of shared memory -- the first real GPU optimization.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}