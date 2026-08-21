# Module 06 — Memory

Where the manual part of C++ starts. Python has a garbage collector; C++ has you.

The goal is a mental model precise enough to answer, for any piece of data: **where does it live, how long does it live, and who is responsible for releasing it?**

## Theory

| File | Topic |
| --- | --- |
| [01-addresses-and-bytes.md](theory/01-addresses-and-bytes.md) | What an address is, and what a byte is |
| [02-stack-and-heap.md](theory/02-stack-and-heap.md) | Two regions, two lifetimes, two costs |
| [03-new-and-delete.md](theory/03-new-and-delete.md) | Dynamic allocation, leaks, dangling pointers |
| [04-locality.md](theory/04-locality.md) | Cache and locality, conceptually |

## Examples

- [`examples/01-stack-vs-heap.cpp`](examples/01-stack-vs-heap.cpp)

## Experiments

The exercises here are **experiments**: print addresses, compare them, allocate, free, measure. Observing real behaviour matters more than writing clever code.

6 exercises in [`exercises/`](exercises/). Exercise 06 is the important one — it is the first cache-locality measurement in this lab.

Run everything under a sanitizer:

```bash
g++ -std=c++17 -Wall -Wextra -g -O0 -fsanitize=address -o prog prog.cpp
```

## Not covered

Smart pointers (`unique_ptr`, `shared_ptr`) are deliberately skipped. They are the right answer in modern C++ and the wrong thing to learn first here, because **CUDA memory management is manual** — `cudaMalloc` and `cudaFree`, with no destructor to help. Learning the manual discipline is the point.

## Checkpoint

[checkpoint.md](checkpoint.md)

## Why This Matters for CUDA

GPU performance is dominated by data movement, not arithmetic. This module is where that stops being a slogan.

| Here | On the GPU |
| --- | --- |
| Stack | Registers — fast, tiny, per-thread |
| Cache | L1 / shared memory — fast, small, per-multiprocessor |
| Heap | Global memory — large, slow, and where the time goes |
| Locality | Coalescing — whether a warp's 32 addresses are contiguous |
| `new` / `delete` | `cudaMalloc` / `cudaFree`, with no garbage collector and less memory |
| A leak | A leak on a 12 GB device, which runs out much faster |

The whole research North Star of this repository — fused normalization kernels — is an argument about **which intermediates get written to memory and which stay in fast storage**. That argument is unreadable without the model this module builds.