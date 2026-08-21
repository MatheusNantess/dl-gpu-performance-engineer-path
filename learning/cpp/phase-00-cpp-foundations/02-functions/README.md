# Module 02 — Functions

One question runs through this entire module:

> **Does the function get a copy, or access to the original?**

In Python you never ask it — everything is a reference to an object, and whether a mutation is visible outside depends on whether the object is mutable. In C++ the answer is visible in the signature, and it decides both correctness and how many bytes get moved.

## Theory

| File | Topic |
| --- | --- |
| [01-signatures.md](theory/01-signatures.md) | Return types, parameters, declarations vs definitions, prototypes |
| [02-stack-frames.md](theory/02-stack-frames.md) | What actually happens on a call |
| [03-passing.md](theory/03-passing.md) | By value, by pointer, by const — the copy question |

## Examples

- [`examples/01-copy-vs-original.cpp`](examples/01-copy-vs-original.cpp)

## Exercises

8 exercises in [`exercises/`](exercises/). Several ask *"will this value change outside the function?"* before any code is written. Answer first, then verify.

## Checkpoint

[checkpoint.md](checkpoint.md)

## Why This Matters for CUDA

A CUDA kernel signature is exactly this question at scale:

```cpp
__global__ void scale(float* data, int n, float factor)
```

`data` is a pointer — no copy, the kernel writes the caller's memory. `n` and `factor` are values — copied to every thread, which is fine because they are 4 bytes each.

Getting this wrong on the GPU does not produce a silently unchanged variable; it produces a kernel that copies a gigabyte for no reason, or one that writes to memory the host cannot see. The habit of reading a signature and knowing immediately what is copied is built here.