/*
Exercise 11 — Uninitialized memory
Difficulty: HARD

Goal:
See what an uninitialized variable actually contains, and why it is dangerous.

Requirements:
- Declare several uninitialized local ints and floats in a function and print
  them. Run the program several times. Record the values.
- Call a second function that declares and sets local variables, then call the
  first function again. Did the printed garbage change? Explain.
- Compile at -O0 and -O2. Compare.
- Add -Wuninitialized (or -Wall) and record the warning.

Questions before coding:
1. Where does the garbage value come from?
2. Why might calling another function first change what you see?
3. Why is reading it undefined behaviour rather than "just some number"?

Why this matters for CUDA:
Device memory from cudaMalloc is likewise uninitialized. Kernels that read
before writing produce results that look plausible, vary between runs, and
are extremely hard to debug.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}