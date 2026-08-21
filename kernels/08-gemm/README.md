# 08 — GEMM

**Backward difficulty:** Easy to derive, hard to make fast

## Operation

General matrix multiplication, the operation underneath every linear layer. Tiled, and eventually mapped onto Tensor Cores.

## Why this kernel

The only compute-bound kernel in the sequence at realistic sizes, and the one with the most demanding optimization ladder: shared memory tiling, register tiling, vectorized loads, layout, Tensor Core MMA, and pipelining.

## Backward

Conceptually simple: the backward is two more matrix multiplications, one for each input. The difficulty is entirely in the layouts, since each of those GEMMs wants a different transposition, and a layout that suits the forward can be actively wrong for the backward.

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

Compute bound at large sizes, memory bound at small ones. Establishing where that crossover sits on this device is part of the work. cuBLAS and CUTLASS are the honest baselines, and losing to them is the expected outcome.

Stated as a hypothesis to be checked against a profile, not as a conclusion.

## Results

No measurements yet.

## Status

Not started. Requires the Tensor Core material from the low-precision phase.

Structure follows [`_template/`](../_template/); subdirectories are created when there is something to put in them.