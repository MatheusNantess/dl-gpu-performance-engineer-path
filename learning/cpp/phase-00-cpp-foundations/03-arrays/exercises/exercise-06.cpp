/*
Exercise 06 — Array to function
Difficulty: MEDIUM

Goal:
See that the function writes the caller's memory.

Requirements:
- Write void zero_out(int* arr, int n) and void fill_sequence(int* arr, int n).
- Write int sum(const int* arr, int n).
- Call all three from main and print the array between calls.
- Try to write through the const pointer in sum. Record the error, then remove it.

Questions before coding:
1. Why does zero_out change the caller's array when a plain int parameter would not?
2. Why does sum take const and zero_out does not?
3. What would happen if you forgot to pass n and used sizeof inside?

Why this matters for CUDA:
This is exactly a kernel signature, minus __global__ and the thread index.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}