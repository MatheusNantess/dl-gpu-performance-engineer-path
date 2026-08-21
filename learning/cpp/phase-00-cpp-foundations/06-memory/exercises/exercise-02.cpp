/*
Exercise 02 — Stack vs heap
Difficulty: EASY

Goal:
Allocate both ways and compare.

Requirements:
- Create a small array on the stack and one on the heap with new[].
- Print both base addresses and the address of a stack int for reference.
- Note how far apart the two regions are.
- delete[] the heap array.
- Then allocate and free in a loop, printing addresses. Do they repeat?

Questions before coding:
1. Roughly how far apart are the stack and heap regions?
2. After delete[] and a new allocation of the same size, do you get the same address?
3. Which allocation is cheaper, and why?

Why this matters for CUDA:
cudaMalloc is the device heap. Understanding that heap addresses are handed out by an allocator explains why the CUDA caching allocator exists and why fragmentation is a real problem.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}