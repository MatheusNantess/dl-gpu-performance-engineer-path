/*
Exercise 04 — Array of structs
Difficulty: MEDIUM

Goal:
Measure the stride, and see the AoS access problem.

Requirements:
- Define a Particle struct with at least six float fields.
- Declare an array of them and print the address of each element.
- Confirm the stride equals sizeof(Particle).
- Then print the addresses of the x field of consecutive elements. What is
  the stride between those?
- Write a loop that sums only the x values and note the access pattern.

Questions before coding:
1. What is the stride when reading only one field of every element?
2. How many useful bytes are in each 64-byte cache line for that loop?
3. How would you change the layout so that loop reads contiguously?

Why this matters for CUDA:
This IS the AoS coalescing problem. On a GPU, 32 threads each reading one field of one struct produce 32 scattered accesses.
*/

#include <iostream>
#include <cstddef>

int main() {
    // TODO: implement
    return 0;
}