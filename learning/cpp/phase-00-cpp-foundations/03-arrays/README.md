# Module 03 — Arrays

**The most important idea in this phase: contiguous memory.**

A C array is not a Python list. It is a block of bytes, one element after another, with nothing in between and no metadata attached. Every performance property that matters on a GPU follows from that one fact.

## Theory

| File | Topic |
| --- | --- |
| [01-c-arrays.md](theory/01-c-arrays.md) | Fixed size, indexing, contiguity |
| [02-sizeof-and-elements.md](theory/02-sizeof-and-elements.md) | `sizeof(array)`, element size, counting elements |
| [03-arrays-in-functions.md](theory/03-arrays-in-functions.md) | Array decay — why the size is lost |
| [04-2d-and-row-major.md](theory/04-2d-and-row-major.md) | 2D arrays and row-major layout |

## Examples

- [`examples/01-contiguity.cpp`](examples/01-contiguity.cpp)
- [`examples/02-row-major.cpp`](examples/02-row-major.cpp)

## Exercises

15 exercises in [`exercises/`](exercises/).

## Checkpoint

[checkpoint.md](checkpoint.md)

## Why This Matters for CUDA

This module is the direct ancestor of **memory coalescing**, the first real GPU optimization.

On a GPU, 32 threads execute together as a warp. When all 32 read *consecutive* addresses, the hardware merges those into a small number of wide memory transactions. When they read scattered addresses, it issues many transactions instead, and the kernel runs at a fraction of the achievable bandwidth.

That is the entire idea, and it is only meaningful if "consecutive addresses" is already concrete. Which is what this module makes it.

Row-major layout matters for the same reason: iterating a matrix along rows walks memory in order, and iterating along columns jumps by the row width every step. On a CPU that costs cache misses. On a GPU it costs most of your bandwidth.