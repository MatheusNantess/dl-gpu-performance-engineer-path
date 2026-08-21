/*
Challenge 08 — Struct memory analysis

Combines: layout, padding, alignment, AoS/SoA

Goal:
Analyse and optimize a layout, then justify it with numbers.

Requirements:
- Define a struct with at least six fields of mixed types, deliberately badly ordered.
- Predict sizeof, then measure. Map every field offset with offsetof.
- Draw the byte layout in a comment, marking padding.
- Reorder to minimize size. Report the saving in bytes and as a percentage.
- For one million elements, report both totals in MB.
- Then build an SoA version and compare a single-field traversal on both, timed.
- State which layout you would choose for: (a) updating all fields of one element,
  (b) reading one field of all elements.

Questions before coding:
1. Predict every sizeof before measuring.
2. How many useful bytes per 64-byte cache line does each layout give for a
   single-field traversal?
3. Which layout would a GPU kernel want, and does that depend on the kernel?

Byte accounting:
State, in a comment with your solution, how many bytes this reads and how
many it writes, for n elements. This is the habit that becomes roofline
analysis later.

Why this matters for CUDA:
Layout choice is one of the standard GPU decisions. NCHW versus NHWC is this exact argument for image tensors, and how a tensor is laid out determines whether a fused normalization kernel can read it efficiently -- which is part of this repository's research question.
*/

#include <iostream>

int main() {
    // TODO: implement
    return 0;
}