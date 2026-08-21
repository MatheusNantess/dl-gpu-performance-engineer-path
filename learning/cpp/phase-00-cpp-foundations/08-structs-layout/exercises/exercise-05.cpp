/*
Exercise 05 — Structs through pointers
Difficulty: MEDIUM

Goal:
Combine Module 04 with layout.

Requirements:
- Write functions taking Particle*, const Particle*, and Particle&.
- Write a function that updates every particle in an array, in place.
- Compute a field offset manually: from a Particle*, reach the y field using
  char* arithmetic and offsetof. Verify it matches the direct access.
- Then write a function taking void* and a stride, walking an array generically.

Questions before coding:
1. Why does the manual offset calculation need a char* cast?
2. What does the void* version give up, and what does it gain?
3. Which of these signatures would appear in a kernel?

Why this matters for CUDA:
Manual offset arithmetic is how strided and layout-generic kernels index their data.
*/

#include <iostream>
#include <cstddef>

int main() {
    // TODO: implement
    return 0;
}