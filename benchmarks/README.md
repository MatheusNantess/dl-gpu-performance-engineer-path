# Benchmarks

Benchmark harness, baseline implementations, and curated results. Everything here follows [docs/benchmark-methodology/](../docs/benchmark-methodology/README.md).

## Layout

```text
benchmarks/
├── harness/       # shared benchmarking code: timing, warmup, sweeps, reporting
├── baselines/
│   ├── pytorch-eager/    # the honest starting baseline
│   ├── torch-compile/    # default and max-autotune
│   ├── triton/           # Triton implementations of the same op
│   ├── cuda/             # CUDA implementations of the same op
│   └── libraries/        # cuBLAS, cuDNN, CUTLASS, FlashAttention
└── results/       # curated, committed results
```

## The harness

One shared harness rather than per-kernel timing code, because the failure modes — missing warmup, missing synchronization, single-shape reporting, mean instead of median — are exactly the ones that repeat when each kernel rolls its own.

The harness is responsible for: warmup, repetition, CUDA-event timing, shape/dtype sweeps, correctness re-verification before timing, and capturing hardware and version metadata into every result.

## Baselines

Ordered by how hard they are to beat:

| Baseline | What beating it proves |
| --- | --- |
| PyTorch eager | Very little on its own — eager leaves obvious bandwidth on the table. It is the starting point, not an achievement |
| `torch.compile` | Something real: Inductor already fuses elementwise chains and generates competent Triton |
| Triton vs CUDA | Where each abstraction's ceiling actually is, on the same operation |
| cuBLAS / cuDNN / CUTLASS / FlashAttention | These are the bar. Losing is the expected outcome for a long time |

**A loss that is precisely explained is a better result than a win that is not.** The point of the library baselines is to find out what they are doing that I am not.

## Results

Raw runs stay local (gitignored). What gets committed to `results/` is curated: the sweep, the methodology used, the hardware and versions, and the shapes where the implementation loses alongside the ones where it wins.

Nothing is projected, estimated, or extrapolated. If a shape was not measured, it does not appear.

Template: [`docs/templates/benchmark-report.md`](../docs/templates/benchmark-report.md)
