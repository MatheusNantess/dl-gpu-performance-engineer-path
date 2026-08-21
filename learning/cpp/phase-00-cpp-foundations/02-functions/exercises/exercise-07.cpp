/*
Exercise 07 — Dangling return
Difficulty: HARD

Goal:
Create the bug deliberately, observe it, and understand why it is not
reliably detectable.

Requirements:
- Write a function that returns the address of a local int. Compile it and
  record the compiler warning.
- Call it and print the value through the returned pointer.
- Then call an UNRELATED function that uses several locals, and print through
  the stale pointer again. Compare.
- Compile at -O0 and -O2 and compare.
- Then write the correct version: have the caller own the variable and pass
  its address in.

Questions before coding:
1. Why does the first read sometimes print the right value?
2. What overwrote it after the second call?
3. Why is "it printed the right value" not evidence that the code is correct?

Why this matters for CUDA:
The device equivalent -- reading shared memory after a block finishes, or a
pointer to memory already freed -- behaves identically: plausible results,
non-deterministic, extremely hard to trace.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}