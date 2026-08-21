/*
Exercise 02 — Pointers to different types
Difficulty: EASY

Goal:
See that a pointer has a type, and what that type controls.

Requirements:
- Declare a char, int, float and double, and a pointer to each.
- Print sizeof for each variable and each pointer.
- Print each pointer value and each dereferenced value.
- Try assigning an int* to a float*. Record the error.

Questions before coding:
1. Do all four pointers have the same size? Why?
2. If pointers are all the same size, what does the pointer TYPE actually control?
3. What would go wrong if you could freely assign int* to float*?

Why this matters for CUDA:
A host float* and a device float* are the same type and are not interchangeable. The type system does not help; the discipline does.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}