/*
Exercise 05 — new/delete pairing
Difficulty: MEDIUM

Goal:
Get the array forms right and see what mismatching does.

Requirements:
- Allocate a single int with new and free it with delete.
- Allocate an array with new[] and free it with delete[].
- Deliberately mismatch: new[] freed with delete. Run under a sanitizer.
- Write allocate/release functions taking float** so they can set and null
  the caller's pointer.
- Verify a full allocate, fill, use, release cycle is leak-free.

Questions before coding:
1. Why do the array forms exist separately?
2. What does the sanitizer say about the mismatch?
3. Why must release take float** rather than float*?

Why this matters for CUDA:
allocate/release taking a double pointer is exactly cudaMalloc/cudaFree. The manual pairing discipline is identical.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}