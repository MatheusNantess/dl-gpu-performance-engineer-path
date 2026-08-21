# Benchmark Methodology

Every performance number in this repository follows this document. A result that does not is not a result.

The purpose is not to produce impressive numbers. It is to produce numbers that are **still true tomorrow, on someone else's machine, under scrutiny**.

## Non-negotiables

### 1. Correctness first

**No timing run happens before correctness passes.** Forward and gradients both. A benchmark of an incorrect kernel is worse than no benchmark, because it looks like information.

Every benchmark script re-verifies correctness on the shapes it is about to time. Correctness and performance drift apart silently otherwise.

### 2. Warmup

The first iterations measure compilation, autotuning, allocator behaviour and clock ramping — not the kernel. Discard them.

- Enough warmup iterations that timings have stabilized, verified by looking at the distribution rather than assumed.
- Triton autotuning and `torch.compile` need substantially more warmup than a plain CUDA kernel; compilation must not land inside the measured region.

### 3. Correct synchronization

GPU work is asynchronous. Timing without synchronization measures launch time, not execution.

- Use CUDA events around the region, or `torch.cuda.synchronize()` before reading a host-side clock.
- Never time an asynchronous launch with a host timer and no sync.
- Be explicit about whether the measurement includes launch overhead — for small kernels that is most of the time, and it is a legitimate thing to measure as long as it is stated.

### 4. Repetitions and distribution

- Multiple repetitions, always.
- Report the **median** as the headline and include the spread. The mean is sensitive to outliers from unrelated system activity.
- If variance is high, say so. High variance is itself a finding — it usually means clock throttling, another process on the GPU, or a shape-dependent code path.
- Note whether GPU clocks were locked. On a consumer card under sustained load, thermal throttling is real and will quietly change results between runs.

### 5. Shape sweeps, not single shapes

One shape is an anecdote. Sweep:

- **Multiple shapes** — including sizes that do not divide evenly by the block size, where the tail handling shows up.
- **Multiple batch sizes** — small batches expose launch overhead, large ones expose bandwidth limits.
- **Multiple dtypes** — FP32, FP16, BF16, and TF32 on/off where relevant.
- **Realistic shapes** — hidden sizes, sequence lengths and batch sizes from actual models, not only round powers of two. Powers of two flatter kernels in ways real workloads do not.

A result presented for one shape must say so explicitly.

### 6. Forward and backward measured separately

They have different bottlenecks and different traffic. Measuring only the combined step hides which half is the problem — usually the backward, since it typically moves about twice the memory.

Report: forward alone, backward alone, and the full step.

### 7. Fair comparison

The most common way benchmarks lie is by comparing unlike things.

- Same dtype, same shapes, same device, same numerical semantics on both sides.
- If the baseline computes something slightly different (different epsilon placement, different accumulation precision, TF32 enabled on one side only), that is stated — or the comparison is not made.
- Baselines get the same warmup and the same measurement treatment.
- The baseline is configured the way a competent user would configure it. Comparing against a deliberately handicapped baseline is not a result.
- `torch.compile` baselines state the mode (`default`, `max-autotune`) and whether compilation was excluded from timing.

### 8. Hardware and software documented

Every result carries:

- GPU model, and clock settings if locked
- Driver version, CUDA toolkit version
- PyTorch version, Triton version
- OS
- Relevant flags: TF32 on/off, cuDNN benchmark mode, deterministic mode

Template in [`docs/hardware/`](../hardware/README.md). Numbers without this context are not portable and will not be presented as if they were.

### 9. Reproducible

Anyone should be able to re-run the benchmark and get comparable numbers:

- Benchmark scripts committed alongside their results.
- Fixed seeds for input generation.
- The exact command recorded with the result.
- Raw output kept locally; the curated summary committed to `benchmarks/results/`.

## Beyond wall clock

Time alone does not explain anything. Where applicable, also report:

- **Achieved memory bandwidth** — and the percentage of the device's *measured* peak, not its datasheet peak.
- **Effective FLOP/s** for compute-bound kernels, likewise against a measured peak.
- **Arithmetic intensity** and the kernel's position on the roofline.
- **Peak memory footprint** — often the actual constraint, especially in the backward.
- **Kernel count and launch overhead** for anything involving fusion.

For a memory-bound kernel, "reached 82% of measured peak bandwidth" is a far more useful statement than a millisecond figure, because it says how much room is left.

## Baselines, in increasing order of difficulty

1. **PyTorch eager** — the honest starting point. Beating it is expected, not impressive.
2. **`torch.compile`** — the real bar for fused elementwise and reduction work.
3. **Triton vs CUDA** — the same operation in both, compared against each other.
4. **Production libraries** — cuBLAS, cuDNN, CUTLASS, FlashAttention. Losing here is the expected outcome for a long time.

**A loss that is precisely explained is a better result than a win that is not.**

## Reporting rules

- Report the shapes where the implementation **loses**, alongside the ones where it wins. A sweep with the bad shapes removed is a fabrication.
- No extrapolation. If a shape was not measured, it does not appear.
- No projected or estimated speedups. Ever.
- State what was *not* tested.
- If a result cannot be explained, it is published as an open question rather than as a finding.

## Report template

[`docs/templates/benchmark-report.md`](../templates/benchmark-report.md)
