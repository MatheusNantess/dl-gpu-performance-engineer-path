/*
Exercise 12 — Bandwidth reasoning
Difficulty: HARD

Goal:
Connect types to the performance argument that runs through this whole lab.

Requirements:
Write a program that, for a given element count n, reports:
- bytes for n floats, and for n doubles
- the theoretical minimum bytes moved by an elementwise operation
  out[i] = a[i] + b[i] over n elements  (think: how many reads, how many writes)
- the time that many bytes would take at a given bandwidth in GB/s, taken as
  a parameter -- do NOT hardcode a number for the RTX 3060, since that value
  has not been measured yet
- the same figures for a normalization-style pass that reads the data twice

Questions before coding:
1. For out[i] = a[i] + b[i], how many bytes per element are moved? Count reads
   and writes separately.
2. If a kernel moves 120 MB and the device sustains X GB/s, what is the floor
   on its runtime?
3. Why is that floor a floor and not a prediction?

Why this matters for CUDA:
This is the arithmetic behind every roofline analysis and every
"percentage of peak bandwidth" claim in this repository. Doing it by hand on
the CPU first means it is never mysterious later.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}