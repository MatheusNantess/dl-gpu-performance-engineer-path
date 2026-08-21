# 13 — Fused Transformer Ops

**Backward difficulty:** Very hard

## Operation

Fusion across other operation boundaries inside a real block, beyond normalization: bias plus dropout plus residual, and whatever else the profile of the assembled block justifies.

## Why this kernel

Tests whether the strategies found for normalization generalize to other fusion opportunities -- which is one of the things that would separate a narrow result from a contribution.

## Backward

Fused backward across operation boundaries, including cases where fusing the backward requires changing what the forward saves. This is the point where forward and backward have to be designed as one artifact.

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

Determined by profiling the assembled block. Candidate fusions come from measured traffic between kernels and measured gaps in the timeline.

Stated as a hypothesis to be checked against a profile, not as a conclusion.

## Results

No measurements yet.

## Status

Not started. Depends on the full transformer block and the ML compilers phase, since the honest comparison is against what Inductor already fuses.

Structure follows [`_template/`](../_template/); subdirectories are created when there is something to put in them.