/*
Exercise 03 — sizeof inventory
Difficulty: EASY

Goal:
Recover an element count from bytes, and see where that breaks.

Requirements:
- Declare arrays of int, float, double and char with different lengths.
- For each, print sizeof(array), sizeof(array[0]) and the derived element count.
- Then pass one array to a function and print sizeof there. Record the result.
- Explain the difference in a comment.

Questions before coding:
1. What value does sizeof print inside the function, and why that number specifically?
2. Does the answer depend on the array type?
3. Why can the compiler know sizeof(array) in main but not in the function?

Why this matters for CUDA:
Every cudaMalloc call is n * sizeof(T). Getting that wrong under-allocates and corrupts device memory silently.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}