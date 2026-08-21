/*
Exercise 20 — Write the CUDA pattern in C++
Difficulty: HARD

Goal:
Assemble the whole host-side CUDA shape, on the CPU.

Requirements:
Write a program that mirrors the standard CUDA workflow using only C++:
  1. llocate -- a function that takes float** and sets the caller's pointer
     via new[]
  2. ill -- populate the array with known values
  3. copy_to and copy_from -- element-by-element copies between two buffers,
     mimicking cudaMemcpy
  4. process -- take (const float* in, float* out, int n) and compute something
  5. elease -- take float** , delete[], and set the caller's pointer to nullptr
- Check for allocation failure.
- Verify no leaks with valgrind or -fsanitize=address.

Questions before coding:
1. Why must allocate and release take float** rather than float*?
2. Which of these five steps has no equivalent in the CUDA workflow, and which
   CUDA step has no equivalent here?
3. What happens if release is called twice?

Why this matters for CUDA:
This IS the CUDA host-side pattern: cudaMalloc, cudaMemcpy, kernel launch, cudaMemcpy back, cudaFree. Writing it in plain C++ first means the CUDA version introduces only one new idea -- that the processing happens on another device.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}