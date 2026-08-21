/*
Challenge 04 — Find max through pointers

Combines: pointer arithmetic, output parameters, edge cases

Goal:
Return both a value and a location, without containers.

Requirements:
- Implement a function returning a POINTER to the maximum element.
- Then one returning the INDEX through an output parameter.
- Then one returning both the max and min in a single pass, via output pointers.
- Handle ties -- decide and document which occurrence wins.
- Handle n == 0.
- Implement at least one version with pointer arithmetic only.

Questions before coding:
1. What should the pointer version return for an empty array?
2. How do you convert the returned pointer into an index?
3. Could this run in parallel? What would each parallel worker produce, and how
   would the results be combined?

Byte accounting:
State, in a comment with your solution, how many bytes this reads and how
many it writes, for n elements. This is the habit that becomes roofline
analysis later.

Why this matters for CUDA:
The last question is the important one: a parallel max is a reduction, and how partial results combine is the central design question of every reduction kernel.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}