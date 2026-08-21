# Research North Star

The long-term scientific goal of this lab. **A direction, not a plan, and not a claim.**

> **Memory-Efficient Fused Normalization Kernels for Transformer Training: Forward and Backward Optimization on Ampere GPUs**

This is a working title for a paper that may or may not turn out to exist. Whether there is a real contribution here is exactly what the lab is meant to find out — and finding out that there is not would also be a legitimate outcome.

## The professional goal comes first

> Become an elite Deep Learning GPU Kernel Performance Engineer capable of designing, profiling and optimizing high-performance forward and backward kernels for real deep learning workloads.

The paper is downstream of that. Every skill it would require — deriving backwards, writing fused kernels, profiling, rigorous benchmarking, reading production implementations — is a skill the engineering goal needs anyway. **If the paper never happens, none of the work is wasted.** That asymmetry is why this is a good North Star: it points the study in a direction where the failure case is still a strong outcome.

## What the work would investigate

Whether specialized fused normalization kernels can be implemented more efficiently on Ampere GPUs than general-purpose framework implementations, in **training** — which means forward *and* backward, not inference alone.

**Candidate operations**, in order of how the lab will reach them:

- RMSNorm — forward and backward
- LayerNorm — forward and backward
- Residual + RMSNorm, and RMSNorm + Residual
- Residual + LayerNorm
- Other fusions that appear in real Transformer blocks
- Possibly normalization + quantization, much later

**Metrics that would matter**, beyond latency:

| Category | Metrics |
| --- | --- |
| Time | Forward latency, backward latency, full training step latency, throughput, tokens/sec |
| Memory | Global memory traffic, peak memory, intermediates stored vs recomputed |
| Execution | Kernel launches, occupancy, register usage, shared memory usage, SM utilization |
| Efficiency | Achieved memory bandwidth as a fraction of measured peak, arithmetic intensity |

Global memory traffic and peak memory matter as much as latency here. A normalization kernel is memory bound; the entire question is about moving less data.

## The question it would answer

Not:

> My kernel is faster than PyTorch.

But:

> **Why** are certain implementations more efficient, **which** bottlenecks are eliminated, and **under what conditions** does a specialized forward/backward strategy beat a general-purpose one?

"Under what conditions" is the part that turns a measurement into a contribution. A speedup at one hidden size on one GPU is an anecdote. A characterization of *where the crossover is and what causes it* is a result.

## Prior art — read this before claiming anything

**Fused normalization kernels are not an unexplored area.** This is the single most important thing to understand about the North Star, and it should shape the research question rather than be discovered late.

Implementations that already exist and must be surveyed, read as source, and benchmarked before any novelty claim:

- **NVIDIA Apex** — fused LayerNorm and RMSNorm CUDA kernels, long-established
- **Liger Kernel** — Triton kernels for RMSNorm, LayerNorm and other Transformer operations, explicitly targeting training memory and throughput
- **The flash-attention repository** — ships a fused dropout/residual/LayerNorm Triton implementation
- **TorchInductor** — already fuses elementwise chains, and can fuse a residual add into a normalization in some cases. Whatever it generates is the honest baseline
- **Transformer Engine**, and other vendor libraries, particularly for the low-precision paths
- Any other production implementation found during the survey

Treat the exact capabilities above as things to **verify by reading the source and benchmarking**, not as established facts to cite. Library capabilities change.

The consequence for the research question: **the gap is unlikely to be "nobody fused these"**. If a contribution exists, it is more likely to be in a specific place those implementations do not cover — a backward strategy, a save-versus-recompute regime, a shape range, a consumer-Ampere-specific constraint, or a characterization that explains *when* each strategy wins. Finding that gap is research work, and it comes after being able to reproduce what already exists.

## What would make it a paper rather than a blog post

A blog post reports a number. A paper explains a mechanism and shows it generalizes. Candidates for real contribution:

- A fusion strategy not already covered by existing implementations
- A backward-specific strategy — reduction structure, gradient accumulation, parameter-gradient handling
- A different point on the save-versus-recompute trade-off, characterized rather than asserted
- A specialized layout or reduction scheme with a stated reason it wins
- A **characterization** of when specialized kernels beat general-purpose ones, as a function of shape, dtype and architecture
- An approach that generalizes across several normalization operations rather than one
- An insight specific to Ampere, with evidence on whether it transfers to other architectures
- **A well-explained negative result** — a strategy that should help and does not, with the mechanism identified

The last one is undervalued and genuinely publishable in systems work. It is also the most likely honest outcome, and planning for it is not pessimism.

## What would be a strong result

Shape of a hypothetical outcome — **no numbers, because none exist yet**:

```text
Strong baseline (best available production implementation)
        ↓
This implementation

Forward latency:          lower
Backward latency:         lower
Peak memory:              lower
Global memory traffic:    lower
Training throughput:      higher
```

With, for every one of those lines: the shapes and dtypes it holds for, the shapes where it does **not** hold, the profiler evidence for the mechanism, and the hardware it was measured on.

**No number appears in this repository until it has been measured.** Not projected, not estimated, not extrapolated from a related result.

## Hardware

The primary lab is a single **RTX 3060 (Ampere GA106, consumer)**. It is a deliberately good place to start: normalization kernels are memory bound, this device's bandwidth is modest, and a kernel that reaches a high fraction of *its* measured peak has demonstrated the same understanding a faster card would.

Being memory-constrained is an advantage for this specific question. Traffic reduction is easier to observe on a device where traffic is the binding constraint.

Later, if results justify the cost, cross-architecture validation:

```text
RTX 3060 (Ampere consumer)
   ↓  does it hold on datacenter Ampere?
A100
   ↓  does it generalize past Ampere?
H100
```

The purpose is not "bigger GPU, better number". It is to determine whether an optimization is a **general principle** or an **artifact of one memory system**. Both answers are informative, and knowing which one applies is part of the contribution.

## Honest risks

Written down now so they are not discovered as disappointments later.

1. **The contribution may not exist.** The area is well worked. That is a real possibility, not false modesty.
2. **Consumer Ampere is not what these kernels are tuned for.** A win on a 3060 may vanish on an A100 with several times the bandwidth. Cross-GPU validation is what distinguishes an insight from an artifact.
3. **Microbenchmark wins routinely disappear at block level.** A normalization kernel is a small fraction of a training step. This is why Phase 12 exists, and why an isolated result is not accepted as sufficient.
4. **The baseline moves.** Inductor and the kernel libraries improve. A result against a year-old baseline is not a result.
5. **Time.** The prerequisites are years of work. The engineering goal has to stay primary, or the paper becomes a reason to avoid building things.

## The rule

The lab's job is not to prove this hypothesis. It is to build the capability to **investigate** it honestly, and to accept whatever the measurements say.

See [research-questions.md](research-questions.md) for the current formulation and [paper-roadmap.md](paper-roadmap.md) for the gates between here and any attempt at it.
