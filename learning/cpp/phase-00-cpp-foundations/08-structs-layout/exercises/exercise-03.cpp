/*
Exercise 03 — Reordering
Difficulty: MEDIUM

Goal:
Reduce a struct's size without removing data.

Requirements:
- Define struct Bad { char a; int b; char c; double d; short e; };
- Predict its sizeof, then measure.
- Reorder the fields to minimize the size. Predict again, then measure.
- Compute the saving as a percentage, and in MB for one million elements.
- Try at least three different orderings and record each size.

Questions before coding:
1. What ordering heuristic did you find, and does it always work?
2. Which single field placement caused the most waste?
3. Is there any cost to reordering?

Why this matters for CUDA:
For an array of a million structs, a badly ordered layout wastes megabytes of bandwidth on every pass.
*/

#include <iostream>
#include <cstddef>

int main() {
    // TODO: implement
    return 0;
}