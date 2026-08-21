# 10 — FlashAttention

**Backward difficulty:** Very hard

## Operation

IO-aware attention: tiled over the sequence dimension with online softmax, never materializing the attention matrix in global memory.

## Why this kernel

The hardest kernel in the plan and the clearest case in deep learning of an algorithm designed around the memory hierarchy rather than around FLOP count. It is also the canonical worked example of the recomputation trade-off the research track is investigating for normalization.

## Backward

The backward cannot store the attention weights either, so it recomputes them tile by tile. This is the sharpest example in the lab of trading arithmetic for memory traffic, and it is where recomputation stops being a concept and becomes an implementation.

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

The claim is about IO, so IO gets measured. Global memory traffic should fall sharply relative to the naive version, and Nsight Compute should confirm it. Block sizes are constrained by shared memory and registers on this device; those constraints get written down rather than worked around silently.

Stated as a hypothesis to be checked against a profile, not as a conclusion.

## Results

No measurements yet.

## Status

Not started. Depends on softmax, attention, and the optimization phase. Target of project 05.

Structure follows [`_template/`](../_template/); subdirectories are created when there is something to put in them.