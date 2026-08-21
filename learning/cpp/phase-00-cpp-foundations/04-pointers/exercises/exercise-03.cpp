/*
Exercise 03 — Swap through pointers
Difficulty: EASY

Goal:
The classic, written from scratch.

Requirements:
- Write void swap(int* a, int* b).
- Call it and verify.
- Then write a version that swaps two float values.
- Then attempt a version taking values instead of pointers, and show it fails.
- Do not use std::swap.

Questions before coding:
1. What does the caller write at the call site, and why the &?
2. What happens if both arguments are the address of the SAME variable?
3. How many bytes are copied into the function frame?

Why this matters for CUDA:
A kernel produces results only by writing through pointers. This is the smallest example of that.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}