/*
Exercise 07 — Scope and shadowing
Difficulty: MEDIUM

Goal:
See scope boundaries and destruction points directly.

Requirements:
- Create nested blocks with variables at each level.
- Deliberately shadow an outer variable, and print both.
- Attempt to use a variable after its block ends. Comment it out and record
  the error message.
- Compile with -Wshadow and record the warning.

Questions before coding:
1. When exactly is the inner variable destroyed?
2. Does a shadowed outer variable still exist while shadowed?
3. In Python, does a loop variable survive the loop? In C++?

Why this matters for CUDA:
GPU memory has several scopes -- register, shared, global -- each with a
different lifetime. Reasoning about scope precisely starts here.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}