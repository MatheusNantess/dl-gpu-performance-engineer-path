/*
Exercise 16 — Null and defensive checks
Difficulty: HARD

Goal:
Make failures loud instead of silent.

Requirements:
- Write a function that takes a pointer and handles nullptr gracefully.
- Deliberately dereference a nullptr in a separate program and observe the crash.
- Then dereference an UNINITIALIZED pointer several times and observe.
- Compare the two failure modes and write down which is preferable and why.
- Show the delete-then-set-to-nullptr habit and what it converts.

Questions before coding:
1. Which fails more reliably: null or uninitialized? Which is more dangerous?
2. What exactly does a segmentation fault mean, technically?
3. Why is 
o segfault not evidence that memory access is correct?

Why this matters for CUDA:
CUDA errors are asynchronous: a kernel fault surfaces at the NEXT CUDA call, in unrelated code. Making failures loud early is the only way to localize them.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}