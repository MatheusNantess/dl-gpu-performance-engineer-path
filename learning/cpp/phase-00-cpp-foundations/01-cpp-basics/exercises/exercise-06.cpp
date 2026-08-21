/*
Exercise 06 — Loop shapes
Difficulty: MEDIUM

Goal:
Fluency with the indexed for loop, which is the shape a kernel takes.

Requirements:
- Print the integers 0..n-1 forward, then backward.
- Print every third element of 0..n-1.
- Sum 0..n-1 with a loop, then verify against the closed-form n*(n-1)/2.
- Write the backward loop using an int index, then try writing it with an
  unsigned index. Record what happens.
- Do not use any STL algorithm.

Questions before coding:
1. `for (unsigned i = n - 1; i >= 0; --i)` -- what happens and why?
2. At what n does the closed-form n*(n-1)/2 overflow a 32-bit int?
3. Does `i` exist after the loop ends?

Why this matters for CUDA:
The indexed loop is what a kernel replaces with a thread index. Complete
fluency here makes that translation obvious.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}