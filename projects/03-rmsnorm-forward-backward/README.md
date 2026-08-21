# 03 — RMSNorm Forward and Backward Optimization on RTX 3060

**Research track:** milestone 1 toward the [North Star](../../research/north-star.md). Clears [Gates 0 and 1](../../research/paper-roadmap.md).

## Goal

Implement RMSNorm end to end — forward and backward, in Triton and CUDA — validate it, benchmark it, profile it, optimize it, and be able to explain every performance number it produces.

This is the first project where the full method is exercised on an operation that matters. RMSNorm is the right target: simple enough that the math is not the obstacle, but it still needs a real reduction, careful numerics, and a backward with a non-trivial parameter-gradient reduction across the batch.

## Progression

Each step gated by the one before it:

```text
PyTorch reference
        ↓
derive forward
        ↓
derive backward          ← by hand, written up, before any kernel code
        ↓
Triton forward
        ↓
Triton backward
        ↓
CUDA forward
        ↓
CUDA backward
        ↓
correctness testing      ← gradcheck; nothing is timed before this passes
        ↓
benchmarking
        ↓
Nsight profiling
        ↓
optimization
        ↓
comparison
```

## What must be understood by the end

Not just "it is faster" — these specific questions, answered with evidence:

- **Why** does each implementation have the performance it has?
- **Where** is the bottleneck, named from profiler metrics rather than guessed?
- **How much memory traffic** actually occurs, and how does that compare to the theoretical minimum computed by hand?
- **Which intermediates are stored**, and what do they cost in bytes?
- **Which could be recomputed instead**, and at which shapes does that become the better trade?
- **Which operations could be fused**, and what would that save?

The last three feed directly into project 08. This project is where the vocabulary for the research question gets built.

## What "done" means

- [ ] Backward derived by hand and written up in `notes/backward-passes/`
- [ ] Forward and backward correct in Triton and CUDA; `gradcheck` passing
- [ ] Benchmarked across hidden sizes, batch sizes, sequence lengths, and FP32/FP16/BF16 — forward and backward separately
- [ ] Profiled with Nsight Compute; achieved bandwidth as a fraction of the device's **measured** peak
- [ ] Theoretical minimum traffic computed by hand and compared against measured traffic
- [ ] Bottleneck named specifically, and the gap between achieved and theoretical explained
- [ ] Compared against PyTorch eager and `torch.compile`, with version, mode, backend, dtype, shape and hardware all stated
- [ ] Write-up including the optimization attempts that failed

## Notes

Backward passes are where kernel work gets serious. Correctness first: no timing runs until gradcheck passes.

The result that matters here is not a speedup. It is being able to say how close this kernel gets to the hardware's ceiling, and precisely what accounts for the remaining distance.

## Status

Not started. Depends on roadmap phases 7 and 8.

---

Kernels inside this project follow the ten-stage lifecycle in [KERNEL_WORKFLOW.md](../../KERNEL_WORKFLOW.md): correctness gates timing, backward is required, and every optimization needs a profiling-backed hypothesis stated beforehand.
