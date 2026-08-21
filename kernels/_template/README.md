# <Kernel Name>

> Template. Copy this directory to start a kernel: `cp -r kernels/_template kernels/NN-name`, then replace everything below.

## Operation

<!-- The mathematical definition. Precise, including shapes and dtypes. -->

## Why this kernel

<!-- What it teaches, and where it appears in real models. -->

## Stage checklist

Stages defined in [KERNEL_WORKFLOW.md](../../KERNEL_WORKFLOW.md).

| # | Stage | Status | Notes |
|---|-------|--------|-------|
| 1 | Correctness baseline          | ☐ | |
| 2 | Forward implementation        | ☐ | |
| 3 | Backward implementation       | ☐ | |
| 4 | Correctness validation        | ☐ | |
| 5 | Benchmarking                  | ☐ | |
| 6 | Profiling                     | ☐ | |
| 7 | Bottleneck analysis           | ☐ | |
| 8 | Optimization                  | ☐ | |
| 9 | vs PyTorch eager              | ☐ | |
| 10 | vs torch.compile / libraries | ☐ | |

## Forward

<!-- Approach, parallelization strategy, why this decomposition. -->

## Backward

<!-- Derivation (or a link to notes/backward-passes/). What forward saves vs recomputes, and why.
     Which gradients need reductions, along which axes. -->

## Correctness

<!-- How it is validated, at what tolerances, and the justification for those tolerances.
     Edge cases covered. -->

## Bottleneck

<!-- Memory bound or compute bound? Which specific limiter, per the profiler? -->

## Results

<!-- Only measured numbers, with hardware and versions. Nothing projected or estimated.
     Include shapes where this implementation loses. -->

## Open questions

<!-- Things not yet understood. Honest and specific. -->

---

## Directory layout

```text
├── forward/
│   ├── baseline/      # slow, obviously correct — the reference everything is checked against
│   ├── triton/
│   └── cuda/
├── backward/
│   ├── baseline/      # reference backward, derived by hand
│   ├── triton/
│   └── cuda/
├── correctness/       # forward tests, gradcheck, edge cases, tolerance rationale
├── benchmarks/        # scripts + recorded results (docs/benchmark-methodology/)
├── profiling/         # profiler output + interpretation
└── optimization/      # one file per attempt: hypothesis → change → measurement → verdict
```

Delete the subdirectories that do not apply. Add them when there is something to put in them, not in advance.
