/*
Exercise 06 — Choosing conventions
Difficulty: HARD

Goal:
Design an interface and defend every choice.

Requirements:
For each of the following, choose value, pointer, const pointer, reference or
const reference, and justify it in a comment covering: is it modified, how
many bytes cross the boundary, can it be absent.

  1. A function computing the length of a 3D vector struct.
  2. A function normalizing that vector in place.
  3. A function summing an array of n floats.
  4. A function that allocates an array and hands it to the caller.
  5. A function that optionally writes a diagnostic value.
  6. A function taking a 4 KB configuration struct, read only.

Then state, for each, what the CUDA equivalent would use and why.

Questions before coding:
1. Which of these are forced by the language rather than by preference?
2. Which would differ between host code and device code?
3. Which choice would waste the most bandwidth if made wrongly?

Why this matters for CUDA:
Kernel signature design is exactly this exercise, where the wrong answer costs bandwidth rather than a few bytes of stack.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}