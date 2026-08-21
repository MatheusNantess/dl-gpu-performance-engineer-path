# 06 — Fused Normalization

**Backward difficulty:** Hard
**Research track:** on the critical path to the [research North Star](../../research/north-star.md).

## Operation

Normalization fused with its neighbours as they actually appear in a Transformer block: residual plus RMSNorm, RMSNorm plus residual, residual plus LayerNorm, and other combinations real architectures use.

## Why this kernel

**The kernel the research North Star is about.** Normalization is almost never applied in isolation in a real block; the residual add before or after it is separate traffic over the same tensor. Whether those can be collapsed, what that does to the backward, and under which conditions it wins is the open question of this lab.

## Backward

Where the actual research question lives. Fusing the forward changes what must be saved for the backward, and the backward of a fused region has its own reduction structure. The save-versus-recompute trade-off, the parameter-gradient reduction, and the fusion boundary all interact here, and the right combination is not obvious in advance.

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

Memory bound. The hypothesis to test is that fusion removes an entire round trip over the activation tensor and one kernel launch, in both directions. Both must be measured directly -- DRAM traffic and launch count -- not inferred from wall clock. Prior art exists and must be surveyed and benchmarked before any novelty is claimed.

Stated as a hypothesis to be checked against a profile, not as a conclusion.

## Results

No measurements yet.

## Status

Not started. Requires LayerNorm and RMSNorm complete through stage 10. This is where the research track stops being preparation. See [research/research-questions.md](../../research/research-questions.md).

Structure follows [`_template/`](../_template/); subdirectories are created when there is something to put in them.