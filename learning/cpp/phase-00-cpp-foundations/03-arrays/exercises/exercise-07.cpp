/*
Exercise 07 — Normalization
Difficulty: MEDIUM

Goal:
A two-pass algorithm -- the shape of every normalization kernel.

Requirements:
- Given a float array, compute its mean, then its standard deviation, then
  normalize every element to (x - mean) / stddev.
- Do it in three separate passes first.
- Then reduce it to two passes. State whether one pass is possible and why.
- Verify the result has mean approximately 0 and stddev approximately 1.

Questions before coding:
1. How many times is the array read, in each version?
2. Why is the result only APPROXIMATELY zero mean? What would make it exact?
3. For n elements, how many bytes does each version move?

Why this matters for CUDA:
This is LayerNorm forward, without the learned parameters. It is kernel 04 in this repository and part of the research North Star. Counting the passes here is exactly the analysis done there.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}