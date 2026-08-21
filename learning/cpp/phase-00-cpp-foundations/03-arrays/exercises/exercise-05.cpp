/*
Exercise 05 — Copy and independence
Difficulty: EASY

Goal:
Prove that two arrays are separate memory.

Requirements:
- Declare a source array and a destination array of the same length.
- Copy element by element with a loop.
- Modify the source afterwards and print both.
- Print the base address of each to confirm they are different blocks.
- Do not use memcpy or std::copy.

Questions before coding:
1. In Python, does  = a for a list copy the data? What about here, for arrays?
2. Can you assign one C array to another with =? Try it and record the error.
3. How many bytes does this copy move in total -- reads and writes?

Why this matters for CUDA:
cudaMemcpy is this loop, across the PCIe bus. Knowing it is a genuine byte-for-byte copy explains why host-device transfers are expensive.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}