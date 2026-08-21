# 12 — Transformer Block

**Backward difficulty:** Very hard
**Research track:** on the critical path to the [research North Star](../../research/north-star.md).

## Operation

A complete transformer block: attention, feed-forward, normalization and residual connections, forward and backward.

## Why this kernel

Where a microbenchmark result has to prove it survives contact with a real workload. Fast kernels do not automatically compose into a fast block, and an isolated normalization win that disappears at block level is not a result worth publishing.

## Backward

The full backward of a block, including how gradients flow through residual connections and what the whole block must retain from forward. Peak memory becomes a first-class constraint alongside time.

## Milestones

| Milestone | Status |
|---|---|
| Forward | Not started |
| Backward | Not started |
| Forward optimization | Not started |
| Backward optimization | Not started |

## Stage checklist

Stages defined in [KERNEL_WORKFLOW.md](../../KERNEL_WORKFLOW.md). No timing before stage 4 passes.

| # | Stage | Status |
|---|-------|--------|
| 1 | Correctness baseline          | ☐ |
| 2 | Forward implementation        | ☐ |
| 3 | Backward implementation       | ☐ |
| 4 | Correctness validation        | ☐ |
| 5 | Benchmarking                  | ☐ |
| 6 | Profiling                     | ☐ |
| 7 | Bottleneck analysis           | ☐ |
| 8 | Optimization                  | ☐ |
| 9 | vs PyTorch eager              | ☐ |
| 10 | vs torch.compile / libraries | ☐ |

## Expected bottleneck

Profile with Nsight Systems before Nsight Compute: at block level the first questions are about gaps, launch overhead and traffic between kernels, not about any individual kernel.

Stated as a hypothesis to be checked against a profile, not as a conclusion.

## Results

No measurements yet.

## Status

Not started. Milestone 4 of the research track -- this is where the fused normalization kernel gets validated on something real. Target of project 06.

Structure follows [`_template/`](../_template/); subdirectories are created when there is something to put in them.