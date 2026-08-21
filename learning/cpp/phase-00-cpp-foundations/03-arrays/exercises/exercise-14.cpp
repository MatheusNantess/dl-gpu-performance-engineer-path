/*
Exercise 14 — Measuring layout cost
Difficulty: HARD

Goal:
The first real measurement in this lab. Do not skip it.

Requirements:
- Allocate a large matrix on the heap (start at 2048x2048 floats).
- Sum all elements twice: once row-major, once column-major.
- Time both with <chrono>, using steady_clock.
- Warm up first: run each once before timing, and time several repetitions,
  reporting the MEDIAN.
- Repeat at 512x512, 2048x2048 and 4096x4096.
- Record the results in checkpoint.md.

Questions before coding:
1. Predict the ratio at each size BEFORE running. Write the predictions down.
2. Why would the ratio change with size? What is the cache doing?
3. Both versions perform identical arithmetic. What exactly costs the extra time?

Why this matters for CUDA:
This is the methodology of this entire repository in miniature: warmup, repetitions, median, prediction before measurement. And the effect being measured is the CPU cousin of memory coalescing.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}