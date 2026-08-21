# 07 — Fused Activations

**Backward difficulty:** Moderate

## Operation

Activations fused with their neighbours: bias plus GELU, and the SwiGLU pattern of two projections combined through a gated activation.

## Why this kernel

Fusion applied to a second family of operations, which tests whether the strategies found for normalization generalize. Real models rarely apply an activation in isolation; the surrounding elementwise work is what generates the traffic.

## Backward

Fusion changes what forward must save, and the fused backward has to reproduce the chain rule across all the fused operations in a single kernel. The saved-versus-recomputed decision now spans several operations at once, and the right answer depends on the shapes.

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

Memory bound. The win should appear as reduced DRAM traffic and fewer kernel launches; both should be measured directly rather than inferred from wall clock.

Stated as a hypothesis to be checked against a profile, not as a conclusion.

## Results

No measurements yet.

## Status

Not started. 

Structure follows [`_template/`](../_template/); subdirectories are created when there is something to put in them.