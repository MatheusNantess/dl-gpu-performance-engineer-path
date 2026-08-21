# 02 — Softmax

**Backward difficulty:** Moderate

## Operation

Row-wise softmax over the last dimension, numerically stabilized by subtracting the row maximum.

## Why this kernel

The first real reduction, and the first place numerical stability is not optional. Online (streaming) softmax computes the maximum and the sum in a single pass and is the foundation FlashAttention is built on.

## Backward

The Jacobian of softmax is dense, and materializing it is both wasteful and unnecessary. The lesson here is the vector-Jacobian product: the gradient reduces to an elementwise expression involving one extra row-wise reduction. Deriving that reduction by hand is the point of this kernel.

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

Memory bound. The question is how few passes over the row are needed and whether the reduction can stay in registers and shared memory rather than round-tripping through global memory.

Stated as a hypothesis to be checked against a profile, not as a conclusion.

## Results

No measurements yet.

## Status

Not started. 

Structure follows [`_template/`](../_template/); subdirectories are created when there is something to put in them.