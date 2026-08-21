/*
Exercise 03 — Large allocation
Difficulty: MEDIUM

Goal:
Find the stack limit empirically.

Requirements:
- Try to declare progressively larger arrays on the stack: 1 KB, 1 MB, 8 MB, 64 MB.
- Find where it crashes. Record the size.
- Check the actual limit with ulimit -s and compare.
- Then allocate the same sizes on the heap and confirm they succeed.
- Check whether new[] returns or throws on failure. Test with an absurd size.

Questions before coding:
1. What is the stack limit on this machine?
2. Does the stack overflow produce a useful error message?
3. What does new[] do when it cannot allocate? How would you handle it?

Why this matters for CUDA:
The RTX 3060 has 12 GB. Knowing what happens when an allocation fails, and checking for it, is the same discipline as checking cudaMalloc return codes.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}