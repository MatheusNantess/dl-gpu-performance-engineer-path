/*
Exercise 15 — Dangling pointer
Difficulty: HARD

Goal:
Create the bug, observe it, and understand why it is intermittent.

Requirements:
- Write a function returning the address of a local. Record the compiler warning.
- Read through the returned pointer immediately. Then call an unrelated
  function with several locals, and read again. Compare.
- Do the same with heap memory: allocate, free with delete[], then read.
- Run all of it under -fsanitize=address and record what it reports.
- Then write the correct version.

Questions before coding:
1. Why did the first read often produce the right value?
2. What overwrote the stack version between the two reads?
3. Why is it printed the right number not evidence of correctness?

Why this matters for CUDA:
The device equivalent -- reading freed device memory, or shared memory after a block finishes -- behaves identically, with no sanitizer running by default.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}