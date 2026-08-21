/*
Exercise 15 — 3D indexing
Difficulty: HARD

Goal:
Generalize the flattening formula, as tensors require.

Requirements:
- Represent a D0 x D1 x D2 tensor as a flat array.
- Derive and implement the flat index formula for (i,j,k).
- Fill it so each element encodes its own coordinates, then verify by reading back.
- Then implement the formula for a 4D tensor (batch, channel, height, width).
- For the 4D case, state which index varies fastest in memory.

Questions before coding:
1. Write the 3D formula from first principles, do not look it up.
2. Which dimension is contiguous? Which has the largest stride?
3. For a (batch, seq, hidden) tensor, which axis should a kernel's threads walk
   along to read contiguously?

Why this matters for CUDA:
Every deep learning tensor is this. NCHW versus NHWC is a layout argument with exactly these formulas behind it, and which axis a kernel walks decides whether it coalesces.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}