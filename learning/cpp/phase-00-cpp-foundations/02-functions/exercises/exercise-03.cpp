/*
Exercise 03 — Overloading
Difficulty: EASY

Goal:
See compile-time dispatch happen.

Requirements:
- Write three overloads of a function named describe: one taking int,
  one taking double, one taking char.
- Each prints which overload ran.
- Call it with: 5, 5.0, 5.0f, 'a', true. Predict which overload each picks
  BEFORE compiling.
- Then try adding an overload that differs only by return type. Record the error.

Questions before coding:
1. Which overload does 5.0f select, and why is that not obvious?
2. Which does true select?
3. Is the choice made at compile time or run time? How do you know?

Why this matters for CUDA:
CUDA uses overloading heavily for typed math functions, and template kernels
specialized per dtype rest on the same mechanism.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}