/*
Exercise 01 — Define and inspect
Difficulty: EASY

Goal:
First contact with struct layout.

Requirements:
- Define structs for a 3D point, an RGB colour (three unsigned chars), and a
  particle with position and velocity.
- Create instances, set fields, print them.
- Print sizeof for each, and the sum of its field sizes.
- Access one through a pointer using both -> and (*p). notation.

Questions before coding:
1. Predict each sizeof before compiling.
2. Which of your structs has padding? Why that one?
3. What is the difference between p->x and (*p).x?

Why this matters for CUDA:
Kernels take small structs as parameters and operate on arrays of them. Layout is the whole content of a struct in kernel code.
*/

#include <iostream>
#include <cstddef>

int main() {
    // TODO: implement
    return 0;
}