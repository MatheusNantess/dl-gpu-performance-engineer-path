/*
Exercise 01 — Stack address map
Difficulty: EASY

Goal:
Observe where stack variables actually land.

Requirements:
- Declare a dozen local variables of different types in one function.
- Print the address of each, in declaration order.
- Declare more inside a nested block and print those too.
- Call a second function that declares its own locals and print those addresses.
- Run several times and compare.

Questions before coding:
1. Do addresses increase or decrease as you declare more variables?
2. Where do the nested-block addresses sit relative to the outer ones?
3. How far apart are one function's locals and the next function's locals?

Why this matters for CUDA:
Per-thread stack space on a GPU is tiny. Kernels declaring large locals spill to slow local memory, which profiling reports directly.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}