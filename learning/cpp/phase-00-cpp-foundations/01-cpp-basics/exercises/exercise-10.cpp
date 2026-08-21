/*
Exercise 10 — Signed/unsigned comparison bug
Difficulty: HARD

Goal:
Reproduce, diagnose and fix a bug that appears in real code.

Requirements:
- Write a function that takes an int n and an unsigned int m and returns
  whether n is less than m.
- Call it with n = -1, m = 1. Predict the result first.
- Explain in a comment what the compiler does to make the comparison happen.
- Fix it so it behaves mathematically. Give at least two different fixes.
- Compile with -Wall -Wextra and record the warning text.

Questions before coding:
1. Which operand gets converted, and in which direction?
2. Why is that the rule, rather than the other way round?
3. Where does this bite in practice? (Hint: what type does sizeof return, and
   what type do people usually write loop counters as?)

Why this matters for CUDA:
Kernel index arithmetic mixes int, unsigned and size_t constantly. This exact
bug produces out-of-bounds access that appears to work until it does not.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}