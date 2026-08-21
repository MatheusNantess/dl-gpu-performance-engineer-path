# 06 — Transformer Block Optimization

## Goal

Optimize a complete transformer block end to end, using everything built in the previous projects, and measure the result at the block level rather than per kernel.

Individual fast kernels do not automatically produce a fast block. This project is about what happens between them: launch overhead, memory traffic across boundaries, layout choices that help one op and hurt the next, and fusion opportunities that only appear when the whole block is in view.

## Planned scope

- Assemble a reference transformer block (attention + MLP + normalization + residuals) in plain PyTorch as the correctness and performance baseline.
- Establish baselines: eager, `torch.compile`, and a naive assembly of my own kernels.
- Profile the whole block with Nsight Systems first — find the gaps, the launch overhead and the traffic between ops before touching any single kernel.
- Apply fusion across operation boundaries where the profile justifies it.
- Evaluate layout and precision choices at block level, where the tradeoffs are visible.
- Re-measure after each change; keep the changes that hold up.

## What "done" means

- Block-level output matching the PyTorch reference within stated tolerances.
- Forward and backward benchmarks against eager and `torch.compile` baselines, across realistic model dimensions.
- A profile-driven narrative of the optimization: what was tried, what the profiler said, what worked, what did not.
- A write-up including the failed optimizations — at this level they are the most informative part.

## Notes

This is the project that most resembles real performance engineering work: an existing model, a profiler, and a search for where the hardware is being wasted. Optimizations get accepted only when measurement backs them, and the failed attempts stay in the record.

## Status

Not started. Depends on most of the roadmap.

---

Kernels inside this project follow the ten-stage lifecycle in [KERNEL_WORKFLOW.md](../../KERNEL_WORKFLOW.md): correctness gates timing, backward is required, and every optimization needs a profiling-backed hypothesis stated beforehand.
