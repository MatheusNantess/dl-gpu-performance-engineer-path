/*
Exercise 10 — 2D as flat
Difficulty: MEDIUM

Goal:
Manual flattening -- how GPU code actually does it.

Requirements:
- Represent a 4x5 matrix as a flat array of 20 ints.
- Fill it so element (r,c) holds r*10+c.
- Print it as a grid using row*cols+col indexing.
- Write get(arr, cols, r, c) and set(arr, cols, r, c, value) helpers.
- Verify a few elements against hand-computed offsets.

Questions before coding:
1. Why does the formula need cols rather than rows?
2. What is the flat index of (2,3)? Compute it before running.
3. What happens if you pass rows where cols is expected? Is the result obviously wrong?

Why this matters for CUDA:
cudaMalloc returns a flat pointer. Every matrix kernel in this lab flattens by hand exactly like this.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}