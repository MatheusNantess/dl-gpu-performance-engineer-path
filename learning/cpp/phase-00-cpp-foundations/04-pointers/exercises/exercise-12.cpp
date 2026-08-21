/*
Exercise 12 — Array of pointers vs pointer to array
Difficulty: MEDIUM

Goal:
Untangle two declarations that look similar and are not.

Requirements:
- Declare int* arr[3]  (array of three pointers).
- Declare int (*ptr)[3] (pointer to an array of three ints).
- Print sizeof for each.
- Use each correctly: store addresses in the first, point at an array with the second.
- Draw the memory layout of each in a comment.

Questions before coding:
1. How much memory does each declaration reserve?
2. Which one holds data, and which holds addresses?
3. Which of the two is closer to how a 2D array works in memory?

Why this matters for CUDA:
Understanding why flat arrays with manual indexing are preferred over arrays of pointers is exactly the contiguity argument that makes GPUs fast.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}