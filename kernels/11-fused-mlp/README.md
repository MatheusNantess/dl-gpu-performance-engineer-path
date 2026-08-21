# 11 — Fused MLP

**Backward difficulty:** Hard

## Operation

The transformer feed-forward block: up and gate projections, activation, down projection, fused as far as is profitable.

## Why this kernel

The first kernel spanning several operations of genuinely different character, mixing compute-bound GEMMs with memory-bound elementwise work. Where to place the fusion boundaries is the entire question.

## Backward

The backward has to carry gradients through the whole fused region, and the fusion choices made in the forward directly constrain what the backward can do. Forward and backward stop being separable design problems here.

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

Mixed. The GEMMs and the elementwise sections have different limiters, so profiling must attribute time to each region separately rather than reporting one number for the block.

Stated as a hypothesis to be checked against a profile, not as a conclusion.

## Results

No measurements yet.

## Status

Not started. Depends on GEMM and fused activations.

Structure follows [`_template/`](../_template/); subdirectories are created when there is something to put in them.