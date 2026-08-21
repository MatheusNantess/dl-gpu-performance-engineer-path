# 05 — RMSNorm

**Backward difficulty:** Moderate
**Research track:** on the critical path to the [research North Star](../../research/north-star.md).

## Operation

Root mean square normalization: scale by the root mean square of the last dimension, then apply a learned weight. No mean subtraction, no bias.

## Why this kernel

LayerNorm without the parts that obscure the structure, and the operation most modern large language models actually use. The best first genuinely serious forward-plus-backward target, and the first milestone of the research track.

## Backward

One reduction instead of three, so the derivation stays readable while still requiring a real weight-gradient reduction across the batch. Deriving it by hand before implementing is expected.

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

Memory bound. Achieved bandwidth relative to the measured device peak is the metric that matters, not FLOPs. How close a hand-written kernel gets to that ceiling is the first real research-track measurement.

Stated as a hypothesis to be checked against a profile, not as a conclusion.

## Results

No measurements yet.

## Status

Not started. Milestone 1 of the research track. Target of project 03.

Structure follows [`_template/`](../_template/); subdirectories are created when there is something to put in them.