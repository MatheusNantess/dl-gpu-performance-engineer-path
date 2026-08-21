/*
Exercise 13 — Predict: strided access
Difficulty: MEDIUM

Goal:
Compute the address sequence of a strided pattern by hand.

Requirements:
- Given float data[64], and access pattern data[i * stride] for i in 0..7:
- BEFORE writing code, compute the byte offsets touched for stride = 1, 2, 4, 8, 16.
- Then write a program that prints the actual addresses and confirms.
- For each stride, state how many distinct 128-byte regions are touched.

Questions before coding:
1. At which stride do the 8 accesses stop fitting in one 128-byte region?
2. If a memory system fetches 128 bytes at a time, how much of each fetch is
   wasted at stride 4?
3. Which stride is the best case, and why is it best?

Why this matters for CUDA:
This IS memory coalescing, computed by hand. A warp is 32 threads; the same arithmetic with 32 accesses decides whether a kernel runs at full bandwidth or a fraction of it.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}