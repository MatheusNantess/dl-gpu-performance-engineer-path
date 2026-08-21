# Module 04 — Pointers

**The most important module in Phase 0.** Everything after it depends on it, and CUDA is unreadable without it.

Take longer here than feels necessary. A pointer is not a difficult idea, but the notation packs four different meanings into two symbols, and fluency means never having to pause and work out which is which.

## Theory

| File | Topic |
| --- | --- |
| [01-address-and-dereference.md](theory/01-address-and-dereference.md) | `x`, `&x`, `p`, `*p`, `&p` — the four meanings |
| [02-pointer-arithmetic.md](theory/02-pointer-arithmetic.md) | `p + 1` and why it is not one byte |
| [03-arrays-and-pointers.md](theory/03-arrays-and-pointers.md) | `a[i]` and `*(a + i)` are the same thing |
| [04-pointers-in-functions.md](theory/04-pointers-in-functions.md) | Output parameters, `const`, ownership |
| [05-null-and-invalid.md](theory/05-null-and-invalid.md) | `nullptr`, dangling pointers, segfaults |

## Examples

- [`examples/01-four-meanings.cpp`](examples/01-four-meanings.cpp)
- [`examples/02-pointer-arithmetic.cpp`](examples/02-pointer-arithmetic.cpp)

## Exercises

20 exercises in [`exercises/`](exercises/). **Many require a written prediction before compiling.** That is not a suggestion — being wrong on paper is how the model gets built, and it is the same discipline used later for profiler metrics.

## Companion material

`c-lessons/01-memory-and-addresses/` (a separate C track, if present in this repo) covers this ground in C with a read-predict-run method. Doing both is worth the time; the C version strips away everything except the memory.

## Checkpoint

[checkpoint.md](checkpoint.md)

## Why This Matters for CUDA

CUDA is pointers. There is no way around it:

```cpp
float* d_data;
cudaMalloc(&d_data, n * sizeof(float));                       // pointer to pointer
cudaMemcpy(d_data, h_data, n * sizeof(float), cudaMemcpyHostToDevice);
scale<<<blocks, threads>>>(d_data, n, 2.0f);                  // pointer to device memory
cudaFree(d_data);
```

Note `&d_data` in the first call: `cudaMalloc` has to *modify* the caller's pointer, so it takes the pointer's address. If Module 02's "the pointer itself is a copy" is not solid, that line is unreadable.

And inside the kernel:

```cpp
int i = blockIdx.x * blockDim.x + threadIdx.x;
if (i < n) data[i] *= factor;
```

`data[i]` is `*(data + i)`. Which address that computes, and whether the 32 addresses computed by a warp are contiguous, is what determines the kernel's speed.

**A host pointer and a device pointer look identical and are not interchangeable.** Dereferencing a device pointer on the host crashes; passing a host pointer to a kernel corrupts memory. Nothing in the type system stops either. This is why the discipline built here matters.