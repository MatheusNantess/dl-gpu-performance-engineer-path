# Challenges

Eight integrative problems. Each combines several modules, and none can be solved with one module's material alone.

Do these **after Module 08**, before the mini-projects.

## Rules

- No `std::vector`, no STL algorithms. Raw arrays and pointers.
- Every solution states, in a comment, how many bytes it reads and writes.
- Answer the questions at the top of each file **before** writing code.
- Verify correctness against hand-computed results before considering anything done.

## The list

| # | Challenge | Combines |
| --- | --- | --- |
| 01 | [Array scaler](challenge-01-array-scaler.cpp) | Pointers, functions, arrays |
| 02 | [Dot product](challenge-02-dot-product.cpp) | Pointers, const, numerics |
| 03 | [Array normalization](challenge-03-normalization.cpp) | Multi-pass algorithms, numerics |
| 04 | [Find max through pointers](challenge-04-find-max.cpp) | Pointer arithmetic, output parameters |
| 05 | [Matrix indexing](challenge-05-matrix-indexing.cpp) | Row-major layout, 2D indexing |
| 06 | [Dynamic vector operations](challenge-06-dynamic-vectors.cpp) | Heap, ownership, lifetime |
| 07 | [Debug memory bugs](challenge-07-debug-memory.cpp) | Every memory failure mode |
| 08 | [Struct memory analysis](challenge-08-struct-analysis.cpp) | Layout, padding, AoS/SoA |

## Why these eight

Challenges 01, 02, 03 and 04 are, with a thread index substituted for the loop, the first four CUDA kernels written in this lab. Getting their CPU forms right — signatures, `const` placement, byte counting — means the GPU versions introduce exactly one new idea.

Challenge 07 is the one that will take longest and teach the most.