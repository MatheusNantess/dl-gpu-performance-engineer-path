# 04 — LayerNorm

**Backward difficulty:** Hard
**Research track:** on the critical path to the [research North Star](../../research/north-star.md).

## Operation

Layer normalization: normalize over the last dimension using its mean and variance, then scale and shift by learned parameters.

## Why this kernel

The canonical hard backward, and one of the two operations the research North Star is about. Widely used, well documented, and demanding enough that getting it right is a real milestone.

## Backward

Three separate reductions in the backward, and the parameter gradients reduce across the batch dimension, which the forward never touches. That cross-batch reduction has no forward analogue and is frequently the bottleneck. It is also where the choice between atomics and a two-pass reduction first becomes real.

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

Memory bound in both directions, with the backward moving substantially more traffic than the forward. Whether the mean and variance are saved or recomputed is a measurable trade-off, and it is one of the specific questions the research track asks.

Stated as a hypothesis to be checked against a profile, not as a conclusion.

## Results

No measurements yet.

## Status

Not started. Milestone 2 of the research track; see [research/paper-roadmap.md](../../research/paper-roadmap.md).

Structure follows [`_template/`](../_template/); subdirectories are created when there is something to put in them.