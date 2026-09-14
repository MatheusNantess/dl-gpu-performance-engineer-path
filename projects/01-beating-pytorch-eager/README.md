# 01 — Beating PyTorch Eager

## Goal

Take a set of common deep learning operations and write custom kernels (CUDA and/or Triton) that outperform PyTorch's eager-mode implementations on my hardware — and, more importantly, be able to explain *why* the win exists.

Eager mode is the honest first baseline: unfused elementwise ops, one kernel launch per operation, every intermediate round-tripped through global memory. The opportunity is usually memory traffic, not arithmetic.

## Planned scope

- Pick a handful of operations where eager is clearly leaving bandwidth on the table (fused elementwise chains, normalization, simple reductions).
- Implement custom kernels for each.
- Validate numerically against the PyTorch reference before any timing is done.
- Benchmark across a realistic range of shapes and dtypes — not one cherry-picked size.
- Profile both versions to attribute the difference to a concrete cause (kernel launches, memory traffic, occupancy, stalls).

## What "done" means

- Correctness tests passing against PyTorch with stated tolerances.
- Benchmarks following `docs/benchmark-methodology/`, with hardware and version info recorded.
- A write-up in `docs/writeups/` explaining the mechanism behind each result, including shapes where my kernel *loses* — those get reported too.

## Notes

Beating eager mode is the easy end of this repo. It is a starting point for methodology, not an achievement to oversell. No numbers go here until they are measured.

## Status

Not started.

---

Kernels inside this project follow the ten-stage lifecycle in [KERNEL_WORKFLOW.md](../../KERNEL_WORKFLOW.md): correctness gates timing, backward is required, and every optimization needs a profiling-backed hypothesis stated beforehand.
