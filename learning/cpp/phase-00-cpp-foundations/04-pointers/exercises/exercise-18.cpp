/*
Exercise 18 — Pointer-based algorithms
Difficulty: HARD

Goal:
Non-trivial algorithms with pointers only.

Requirements:
- Implement, using pointer arithmetic and no [] anywhere:
    * partition an array around a pivot value, in place
    * merge two sorted arrays into a third
    * find the longest run of equal consecutive values, returning a pointer to
      its start and its length through an output parameter
- Verify each against hand-computed results.

Questions before coding:
1. In partition, how do you express the two moving positions as pointers?
2. In merge, what are the loop termination conditions?
3. Which of these could be done in parallel, and which are inherently sequential?

Why this matters for CUDA:
The last question is the one that matters: recognizing which loops parallelize is the first step in thinking about what a kernel can do.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}