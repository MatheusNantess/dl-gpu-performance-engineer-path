# Mini Project 1 — CPU Array Kernel Lab

A conceptual preview of the first CUDA kernels, written on the CPU.

Six operations. Each one, later in this lab, becomes a real GPU kernel with an almost identical signature. The only change will be replacing the loop with a thread index and adding `__global__`.

## Operations

| Function | Signature |
| --- | --- |
| `vector_add` | `void vector_add(const float* a, const float* b, float* out, int n)` |
| `scale` | `void scale(float* data, int n, float factor)` |
| `sum` | `float sum(const float* data, int n)` |
| `max_element` | `float max_element(const float* data, int n)` |
| `dot_product` | `float dot_product(const float* a, const float* b, int n)` |
| `normalize` | `void normalize(float* data, int n)` |

## Constraints

- Raw arrays and pointers only. **No `std::vector`, no STL algorithms.**
- Heap allocation for anything large.
- `const` on every parameter that is not written.
- Every function handles `n == 0` and null pointers sensibly.

## Files

| File | Contents |
| --- | --- |
| `kernels.h` | Declarations for all six |
| `kernels.cpp` | Definitions — the work |
| `main.cpp` | Driver: allocate, fill, call, verify, report |
| `verify.cpp` | Independent reference implementations for checking |

Building this as four files rather than one is deliberate: it exercises Module 07, and it mirrors how a CUDA project is organized.

## Required deliverables

Beyond working code:

**1. Byte accounting.** For each operation, state in a comment how many bytes it reads and writes for `n` elements. For example, `vector_add` reads `2n` floats and writes `n`, so it moves `12n` bytes.

**2. Correctness verification.** Every operation checked against an independent implementation in `verify.cpp`, with an explicit tolerance for the floating-point ones. State the tolerance and why it is that value.

**3. Timing.** Time each operation at several sizes, with warmup and repetitions, reporting the median. Use `<chrono>` and `steady_clock`.

**4. A short write-up** in `RESULTS.md`:
- The measured time for each operation at each size
- The bytes moved, and the implied bandwidth in GB/s
- Which operations are memory bound, and how you can tell
- Which would benefit most from being on a GPU, and why

Deliverable 4 is the point of the whole project.

## Build

```bash
g++ -std=c++17 -Wall -Wextra -g -O0 -c kernels.cpp -o kernels.o
g++ -std=c++17 -Wall -Wextra -g -O0 -c verify.cpp  -o verify.o
g++ -std=c++17 -Wall -Wextra -g -O0 -c main.cpp    -o main.o
g++ kernels.o verify.o main.o -o lab

# and for the timing runs -- never benchmark at -O0
g++ -std=c++17 -Wall -Wextra -O2 kernels.cpp verify.cpp main.cpp -o lab-O2
```

## Questions to answer in RESULTS.md

1. Which operations read more than they write? Which write more?
2. `sum` and `dot_product` both reduce to one value. Which moves more bytes, and by what factor?
3. `normalize` requires more than one pass. How many, and can it be fewer?
4. Compute arithmetic intensity (FLOPs per byte) for each. Which is highest?
5. Which of these six is the best candidate for GPU acceleration, and which is the worst? Justify with the numbers, not intuition.
6. Compare `-O0` and `-O2` timings. Which operations gained most, and why?

## Why This Matters for CUDA

These six are, almost exactly, `fundamentals/01` through `05` and `kernels/01` through `03` in this repository.

Writing them on the CPU first means that when the GPU versions arrive, the algorithm, the signature, the correctness check and the byte accounting are all already familiar. The only new idea will be that the loop is gone and the index comes from the hardware.

The byte accounting in particular is the beginning of roofline analysis, which is how every kernel in this lab gets evaluated.