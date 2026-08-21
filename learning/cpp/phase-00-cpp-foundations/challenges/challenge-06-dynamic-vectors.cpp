/*
Challenge 06 — Dynamic vector operations

Combines: heap allocation, ownership, lifetime

Goal:
Manage memory manually across a whole set of operations.

Requirements:
- Write functions that allocate, fill, operate on, and free float arrays of a
  size given at run time.
- Implement: create, destroy, copy, add, subtract, scale, and a length/norm.
- create and destroy must take float** so they can set and null the caller''s pointer.
- Check every allocation for failure.
- Verify with -fsanitize=address that there are no leaks and no invalid access.
- Document, for each function, who owns the returned memory.

Questions before coding:
1. Why must create and destroy take a double pointer?
2. What happens if destroy is called twice? Make it safe.
3. Which operations could work in place, and what is the trade-off?

Byte accounting:
State, in a comment with your solution, how many bytes this reads and how
many it writes, for n elements. This is the habit that becomes roofline
analysis later.

Why this matters for CUDA:
This is the CUDA host-side pattern exactly: cudaMalloc, use, cudaFree, with the caller owning the memory and no destructor to help.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}