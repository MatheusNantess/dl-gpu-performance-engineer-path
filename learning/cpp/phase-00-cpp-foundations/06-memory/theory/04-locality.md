# 04 — Locality and Cache

Conceptual. Enough to explain measurements, no more.

## The problem

RAM is far slower than the CPU. Fetching from main memory takes on the order of a hundred nanoseconds; an arithmetic instruction takes a fraction of one. Without help, the processor would spend nearly all its time waiting.

## Cache

A small, fast memory between the processor and RAM, holding recently used data. Typically three levels — L1 smallest and fastest, L3 largest and slowest — with each level larger and slower than the one above.

## Cache lines

**Memory is never fetched one byte at a time.** The cache fetches a whole *line*, typically 64 bytes.

```text
Reading one float from an uncached region:

    fetches 64 bytes
    +----+----+----+----+ ... +----+
    | f0 | f1 | f2 | f3 |     | f15|      16 floats in one line
    +----+----+----+----+ ... +----+
      ^
      you asked for this one; you got all sixteen
```

If the next 15 floats are then read, they are already there — free. If instead the next access is 1000 elements away, that line was fetched to serve one value and the rest was wasted.

That single fact explains every measurement in this module.

## Two kinds of locality

**Spatial** — accessing addresses near each other. Sequential array traversal is the ideal case.

**Temporal** — reusing the same data soon. A value read twice in quick succession is still cached the second time.

## The consequence

```cpp
// good: every fetched line is fully used
for (int i = 0; i < n; ++i) sum += a[i];

// bad: one useful value per fetched line
for (int i = 0; i < n; i += 16) sum += a[i];

// worst: no reuse at all
for (int i = 0; i < n; ++i) sum += a[random_index[i]];
```

Same additions. Very different runtimes. **Exercise 06 measures this**, and doing so is more convincing than any explanation.

## Why row-major traversal wins

From Module 03: walking a matrix along rows follows memory in order, so each cache line is fully consumed. Walking down columns jumps by the row width every step, so each access may fetch a fresh line to use one value from it.

## Memory-bound versus compute-bound

If a loop does little arithmetic per byte loaded, the processor waits on memory and the loop is **memory bound**. Optimizing the arithmetic changes nothing; only reducing or reordering the memory traffic helps.

That distinction is the first question asked of any kernel in this repository, and the roofline model in Phase 1 is its formal statement.

## Why This Matters for CUDA

Everything here transfers, with different numbers and a sharper penalty:

| CPU | GPU |
| --- | --- |
| Cache line, 64 bytes | Memory transaction, typically 32-byte sectors |
| Spatial locality | **Coalescing** — 32 threads reading contiguous addresses |
| Cache | L1, L2, and programmer-managed shared memory |
| Prefetching | Latency hiding by running many warps |
| Memory bound | Memory bound — which most deep learning kernels are |

The largest difference: a GPU has far more bandwidth *and* far more threads demanding it, so the ratio of compute to memory is worse. Which is why almost every kernel in this lab turns out to be memory bound, and why "reduce the bytes moved" is the recurring answer.

Normalization kernels — the research target of this repository — do almost no arithmetic per byte. They are pure memory-traffic problems. This file is the beginning of understanding why.