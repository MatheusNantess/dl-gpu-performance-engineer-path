# Deep Learning Performance Lab

> **North Star** — Become an elite Deep Learning GPU Kernel Performance Engineer capable of designing, profiling and optimizing high-performance **forward and backward** kernels for real deep learning workloads.

This is not a CUDA exercise repository. It is a long-term lab for building expertise in **high-performance deep learning GPU kernels, with emphasis on optimization and backward kernels**.

Target role: **Deep Learning GPU Kernel Performance Engineer**, progressing toward **Deep Learning Kernel Software Performance Architect**.

## Research North Star

Beyond the professional goal, this lab has a long-term scientific direction: building enough understanding to eventually attempt work along the lines of

> **Memory-Efficient Fused Normalization Kernels for Transformer Training: Forward and Backward Optimization on Ampere GPUs**

**This is a direction, not a claim.** Whether a real contribution exists there is exactly what the lab is meant to find out — and concluding that it does not would be a legitimate outcome. Fused normalization kernels already exist in several production libraries; any contribution has to be located relative to them, which means reading and benchmarking them first.

The engineering goal comes first, and the two are aligned: every skill the paper would require — deriving backwards, writing fused kernels, profiling, rigorous benchmarking, reading production implementations — is a skill the engineering goal needs anyway. **If the paper never happens, none of the work is wasted.**

Details, prior art, honest risks and kill criteria: [`research/`](research/README.md).

## Focus

| Area | What it means here |
| --- | --- |
| Deep learning kernels | Softmax, normalization, activations, cross entropy, GEMM, attention, fused blocks |
| **Backward kernels** | Gradient derivations implemented as kernels — first-class, not an afterthought |
| Performance optimization | Driven by profiling and metrics, never by guessing |
| Kernel fusion | Eliminating intermediate tensors and global memory round-trips |
| Memory optimization | Coalescing, shared memory, layout, vectorized access, traffic reduction |
| GPU profiling | Nsight Compute, Nsight Systems, PyTorch Profiler |
| Tensor Cores | MMA shapes, layout requirements, achieved utilization |
| Mixed precision | FP16, BF16, TF32, later FP8 — and the accuracy cost of each |
| Numerical correctness | Validated against PyTorch, forward *and* gradients, before any timing |
| PyTorch internals | Dispatcher, autograd, custom ops, `torch.compile` / Inductor |
| Triton and CUDA | Both, compared directly on the same operation |
| Real workloads | Training and inference shapes, not toy sizes |

## Principles

> **Correctness first. Measure second. Optimize third.**

> **Every important concept should eventually become code, a benchmark, a profiling experiment, or a technical write-up.**

A kernel is **not done because it runs**. It is done when it has passed the lifecycle in [KERNEL_WORKFLOW.md](KERNEL_WORKFLOW.md) — including the backward pass, gradient validation, profiling, and comparison against strong baselines.

## The kernel lifecycle

```text
Understand the operation
        ↓
Implement a correct baseline
        ↓
Implement the forward kernel
        ↓
Validate numerical correctness
        ↓
Implement the backward kernel
        ↓
Validate gradients against PyTorch
        ↓
Benchmark
        ↓
Profile
        ↓
Identify bottlenecks
        ↓
Optimize
        ↓
Benchmark again
        ↓
Compare against strong baselines
```

Full stage definitions, exit criteria and the per-kernel directory layout: [KERNEL_WORKFLOW.md](KERNEL_WORKFLOW.md).

## Optimization is not trial and error

Every optimization in this repo must be justified by a measurement and a hypothesis:

```text
Measure → Profile → Understand → Form a hypothesis → Optimize → Measure again
```

An optimization that made things faster for reasons I cannot explain is logged as an open question, not as a result. Failed optimizations stay in the record — at kernel level they are usually where the understanding is.

The catalog of optimization concepts, what each one costs, and which profiler metric exposes it: [notes/optimization/](notes/optimization/README.md).

## Baselines

Nothing is claimed to be fast in isolation. Comparisons are made against, in increasing order of difficulty:

1. **PyTorch eager** — the honest starting baseline
2. **`torch.compile` / Inductor** — the real bar for fused elementwise and reduction work
3. **Triton** and **CUDA** implementations of the same operation, compared against each other
4. **Production libraries** — cuBLAS, cuDNN, CUTLASS, FlashAttention, as the work justifies it

Benchmarks follow [docs/benchmark-methodology/](docs/benchmark-methodology/README.md): multiple shapes, multiple batch sizes, multiple dtypes, warmup, repeated runs, correct device synchronization, verified correctness, documented hardware, reproducible.

No cherry-picked shape is presented as a general result. Shapes where my implementation loses are reported alongside the ones where it wins.

## Progression this repo should demonstrate

```text
I can write a GPU kernel
        ↓
I can write a correct Deep Learning kernel
        ↓
I can implement its backward pass
        ↓
I can profile it
        ↓
I can explain why it is slow
        ↓
I can optimize it
        ↓
I can compete with framework implementations
        ↓
I can optimize real Deep Learning workloads
        ↓
I can design high-performance forward and backward kernels
```

## Hardware

The lab runs on a single **NVIDIA RTX 3060** (Ampere, GA106, consumer). It has 3rd-gen Tensor Cores, BF16 and TF32 support, and modest memory bandwidth — which makes it a genuinely good machine for learning, because most deep learning kernels are memory bound and that is where the lessons are.

Known limits of this device, stated up front rather than worked around silently: **no FP8** (Ada/Hopper only), **single GPU** (no NVLink, no real distributed work), less shared memory per SM than data center parts. Work needing those runs on cloud instances later; `low-precision/fp8/` and `distributed/` are expected to stay empty until then.

Every benchmark and profiling result records the GPU, driver, CUDA and framework versions it was produced on. Numbers are not portable across GPUs and will never be presented as if they were.

## The seven activities

These are distinct, and confusing them is the main way a lab like this goes wrong. The dependency runs top to bottom.

| Activity | Directory | Question it answers | Output |
| --- | --- | --- | --- |
| **Learning** | `learning/`, `notes/`, `fundamentals/` | How does this work? | Courses, notes, exercises, derivations |
| **Kernels** | `kernels/` | Can I implement this correctly and make it fast? | Validated, optimized forward and backward kernels |
| **Experiments** | `experiments/` | What does *one technique* cost or gain, in isolation? | A measured answer to a narrow question |
| **Benchmarks** | `benchmarks/` | How fast is it, fairly measured? | Numbers with methodology and hardware attached |
| **Profiling** | `profiling/` | *Why* is it that fast? | Bottleneck attribution backed by metrics |
| **Projects** | `projects/` | Can I build something substantial and explain it? | Portfolio-facing write-ups |
| **Research** | `research/` | Is there something here that is not already known? | Refined questions, falsifiable hypotheses |

Research is last for a reason: a research question asked before the measurement skills exist produces an opinion, not a finding.

## Repository map

| Path | Purpose |
| --- | --- |
| [`kernels/`](kernels/README.md) | **The core.** Deep learning kernels, forward and backward, through the full lifecycle |
| [`research/`](research/README.md) | The scientific track: North Star, open questions, gates to a paper |
| [`notes/optimization/`](notes/optimization/README.md) | The optimization knowledge base — concepts, costs, and how each is measured |
| [`notes/backward-passes/`](notes/backward-passes/README.md) | Gradient derivations and the mechanics of backward kernels |
| `c-lessons/` | Guided, annotated C files — the memory-level foundation CUDA assumes (Phase 0) |
| [`learning/`](learning/README.md) | Structured courses with exercises and checkpoints — currently [Phase 0: C++ Foundations](learning/cpp/phase-00-cpp-foundations/) |
| [`fundamentals/`](fundamentals/README.md) | Core GPU programming exercises that everything else assumes |
| [`profiling/`](profiling/README.md) | Profiling runs and reports (Nsight Compute, Nsight Systems, PyTorch Profiler) |
| [`benchmarks/`](benchmarks/README.md) | Benchmark harness, baselines and curated results |
| [`experiments/`](experiments/README.md) | Isolated studies of one optimization technique at a time |
| [`cuda-lab/`](cuda-lab/README.md), [`triton-lab/`](triton-lab/README.md) | Language-specific labs, for direct CUDA vs Triton comparison |
| [`pytorch-lab/`](pytorch-lab/README.md) | PyTorch internals: custom ops, autograd, `torch.compile`, Inductor |
| [`projects/`](projects/README.md) | Larger end-to-end projects with a defined goal |
| [`low-precision/`](low-precision/README.md) | FP16, BF16, FP8, quantization |
| [`distributed/`](distributed/README.md) | NCCL, multi-GPU, tensor and pipeline parallelism |
| [`notes/`](notes/README.md) | Study notes per domain — the theory layer |
| [`scripts/`](scripts/README.md) | Shared tooling for benchmarking and profiling |
| [`docs/`](docs/README.md) | Write-ups, benchmark methodology, hardware records, templates |

## Navigation

- [ROADMAP.md](ROADMAP.md) — 18 phases with completion criteria. **Start at the "Current focus" block**, which always names the next three concrete actions
- [KERNEL_WORKFLOW.md](KERNEL_WORKFLOW.md) — the lifecycle every kernel goes through, and what "done" means
- [research/](research/README.md) — the scientific track: [North Star](research/north-star.md), [open questions](research/research-questions.md), [gates and kill criteria](research/paper-roadmap.md)
- [LEARNING_LOG.md](LEARNING_LOG.md) — dated log of what was studied, run, and measured
- [RESOURCES.md](RESOURCES.md) — curated references per domain

## Additional principles

> **Never optimize without measuring.**

> **A faster kernel is interesting. Understanding why it is faster is more important.**

> **Microbenchmark improvements matter only if they can eventually be connected to real workloads.**

> **Forward performance is not enough. Training requires backward performance.**

## Status

Early stage. Structure and conventions are in place; kernels are being built one at a time through the full lifecycle rather than many at once through the first stage. No performance claim appears in this repository without a reproducible benchmark and a documented methodology behind it.
