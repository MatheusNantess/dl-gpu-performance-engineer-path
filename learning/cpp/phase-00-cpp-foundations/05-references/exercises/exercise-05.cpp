/*
Exercise 05 — Swap, three ways
Difficulty: MEDIUM

Goal:
One task, all three calling conventions.

Requirements:
- Implement swap by value (show it fails), by pointer, and by reference.
- Verify each.
- Write a comment comparing the three call sites.
- Then implement a swap that works on two elements of an array, given the
  array and two indices. Which convention does that need?

Questions before coding:
1. Why can the by-value version never work?
2. Which version reads better at the call site? Which is more explicit?
3. For the array version, why is the answer forced?

Why this matters for CUDA:
Kernels write results through pointers. The reference version has no device equivalent, and knowing why is the point.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}