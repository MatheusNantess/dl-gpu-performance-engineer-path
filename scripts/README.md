# Scripts

Shared tooling. This directory exists so that benchmarking and profiling are done the same way every time — the failure modes (missing warmup, missing synchronization, single-shape reporting) are exactly the ones that recur when each kernel improvises its own.

```text
benchmark/    # timing harness, sweep drivers, result formatting
profiling/    # Nsight Compute / Nsight Systems capture commands, metric extraction
utilities/    # device info, environment capture, correctness helpers
```

## Priorities

The first two things worth writing, before any kernel work:

1. **Environment capture** — a script that records GPU, driver, CUDA, PyTorch and Triton versions and relevant flags, so every result carries its context automatically rather than by discipline.
2. **The timing harness** — warmup, repetitions, CUDA-event timing, median and spread, shape and dtype sweeps, correctness re-verification before timing.

Getting these right once removes an entire category of invalid results from everything that follows.

Profiling commands live here rather than in shell history so that a capture can be reproduced instead of remembered.