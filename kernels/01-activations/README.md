# 01 — Activations

**Backward difficulty:** Trivial (elementwise)

## Operation

Elementwise activation functions: ReLU, GELU (exact and tanh approximation), SiLU/Swish. Pointwise, no reduction, no data reuse.

## Why this kernel

The whole lifecycle end to end on the simplest possible operation. Because the math is trivial, all the attention goes to the process: baseline, validation, benchmarking methodology, profiling, bottleneck analysis. It establishes the template every later kernel copies.

## Backward

Pure elementwise derivative. The only real decision is what forward saves: the input, the output, or neither. For GELU the derivative is expensive enough that saving versus recomputing is a genuine trade-off even here.

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

Memory bound, with arithmetic intensity near zero. Performance should be a direct function of achieved DRAM bandwidth, which makes this a good calibration kernel for the benchmarking harness.

Stated as a hypothesis to be checked against a profile, not as a conclusion.

## Results

No measurements yet.

## Status

Not started. 

Structure follows [`_template/`](../_template/); subdirectories are created when there is something to put in them.