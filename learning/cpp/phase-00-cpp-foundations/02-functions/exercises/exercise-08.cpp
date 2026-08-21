/*
Exercise 08 — Signature design
Difficulty: HARD

Goal:
Design signatures the way kernel authors do, then defend the choices.

Requirements:
Write correct C++ signatures (declarations only, plus one implementation of
your choosing) for each of the following. For EACH one, write a comment
justifying every parameter: copy or original, const or not, and how many
bytes cross the boundary.

  1. Add two arrays of n floats elementwise into a third.
  2. Scale an array in place by a factor.
  3. Compute the sum of an array without modifying it.
  4. Normalize an array in place, given a precomputed mean and standard deviation.
  5. Compute the mean AND standard deviation of an array, returning both.
  6. Copy n floats from one array to another.

Then, for each, state which parameters would be identical in a CUDA kernel
and which would differ.

Questions before coding:
1. Which of these can be const-correct in more than one way?
2. Which need a separate output array and which can work in place? What is the
   trade-off?
3. In (5), why does the array parameter differ from the two outputs?

Why this matters for CUDA:
These six are, almost exactly, the first kernels written in this lab.
Getting the CPU signatures right now means the CUDA versions differ only by
__global__ and an index calculation.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}