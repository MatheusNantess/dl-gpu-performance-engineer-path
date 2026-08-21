/*
Exercise 04 — const and mutation
Difficulty: EASY

Goal:
Find out exactly what const forbids.

Requirements:
- Declare a const int and a non-const int.
- Attempt to modify each.
- Comment out the illegal line, but record the EXACT compiler error message
  in a comment.
- Then declare a const variable initialized from a run-time value (something
  read with std::cin). Does that compile? Should it?

Questions before coding:
1. Is const a compile-time or run-time restriction?
2. Can a const variable be initialized from a value not known until run time?
3. What is the difference between const and constexpr? (One line; do not go deep.)

Why this matters for CUDA:
`const float* input` in a kernel signature says "read only". The compiler
uses that to optimize, and on a GPU it can enable a faster read path.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}