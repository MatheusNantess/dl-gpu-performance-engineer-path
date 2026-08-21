# 07 — LayerNorm Forward and Backward

**Research track:** milestone 2 toward the [North Star](../../research/north-star.md).

## Goal

Repeat the methodology of project 03 on LayerNorm, then compare the two operations directly. The comparison is the point — doing LayerNorm in isolation would mostly repeat work already done.

## Planned scope

**Implementation**
- Derive the LayerNorm backward by hand: gradients w.r.t. input, weight and bias.
- Triton and CUDA, forward and backward.
- Validate including `gradcheck` before any timing.

**Comparison against RMSNorm** — the actual deliverable:

| Dimension | Question |
| --- | --- |
| Reductions | LayerNorm needs mean and variance; RMSNorm needs one reduction. What does the extra reduction cost, measured? |
| Memory traffic | Does the difference in reductions show up as a difference in traffic, or is it absorbed? |
| Numerical stability | Where do the two differ, particularly in low precision and particularly in the backward? |
| Parameter gradients | LayerNorm has bias gradients as well as weight gradients. What does the extra cross-batch reduction cost? |
| Backward complexity | Three reductions versus one — does it show up proportionally in the profile? |

## What "done" means

- Forward and backward correct, gradcheck passing.
- Benchmarks across hidden sizes, batch sizes, sequence lengths and dtypes, forward and backward separately.
- Profiling showing achieved bandwidth against the device's measured peak for both operations.
- A write-up that answers each row of the table above with evidence.

## Why this matters for the research track

The North Star covers both operations. Knowing precisely where their costs diverge is what makes it possible to say whether a fusion strategy is general or specific to one of them — which is one of the things that would separate a contribution from a narrow result.

## Status

Not started. Depends on project 03.

---

Kernels inside this project follow the ten-stage lifecycle in [KERNEL_WORKFLOW.md](../../KERNEL_WORKFLOW.md): correctness gates timing, backward is required, and every optimization needs a profiling-backed hypothesis stated beforehand.