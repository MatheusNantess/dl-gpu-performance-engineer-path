/*
Exercise 05 — Swap
Difficulty: MEDIUM

Goal:
The canonical demonstration that pass-by-value cannot do certain jobs.

Requirements:
- Write swap_by_value(int a, int b) that tries to swap. Show that it fails.
- Write swap_by_pointer(int* a, int* b) that works.
- Print the variables before and after each call.
- Then write swap_by_pointer WITHOUT a temporary variable (using arithmetic
  or XOR). Note at least one case where that version is wrong.

Questions before coding:
1. Why can the by-value version never work, regardless of implementation?
2. What does the caller have to write differently for the pointer version?
3. In the no-temporary version, what happens if both pointers point at the
   SAME variable? Predict, then test.

Why this matters for CUDA:
Kernels communicate results exclusively by writing through pointers. A kernel
that "returns" a value is a kernel that writes to out[i].
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}