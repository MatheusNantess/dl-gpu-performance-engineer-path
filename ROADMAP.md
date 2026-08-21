# Roadmap

**Professional goal:** Deep Learning GPU Kernel Performance Engineer — able to design, implement, analyze and optimize high-performance deep learning kernels, forward *and* backward.

**Research North Star:** eventually be capable of producing [*Memory-Efficient Fused Normalization Kernels for Transformer Training*](research/north-star.md). A direction, not a promise.

Every kernel in this roadmap goes through the ten stages in [KERNEL_WORKFLOW.md](KERNEL_WORKFLOW.md). "Studied it" is never a completion criterion.

---

## Current focus

**Starting point:** Python at intermediate level, no C or C++ at all.

That combination sets the order. Python fluency means Triton and PyTorch reference implementations are available immediately — no language barrier between me and a working kernel. The missing piece is not C++ the language; it is the **memory model** underneath it, which is what CUDA actually requires.

> **Learn C, not C++. Two weeks, then start CUDA.**

Early CUDA kernels are C with extra keywords. Vector add, transpose and reductions need pointers, arrays and index arithmetic — no classes, no templates, no RAII, no move semantics. Modern C++ becomes necessary at Phase 10, for PyTorch extensions, and is deliberately deferred until then.

**The next four concrete actions, in order:**

1. **Get the environment working end to end** — `nvcc` compiling, PyTorch with CUDA, Triton, and Nsight Compute able to read performance counters. On a consumer GPU this last one usually needs counter access enabled in the driver settings. Decide Windows-native vs WSL2 now; migrating later costs more than choosing now.
2. **Two weeks of C** — pointers, pointer arithmetic, arrays and memory layout, stack vs heap, `malloc`/`free`, structs, the compilation model, `const` and `restrict`. Written in C, not C++. Goal: `int *p = &x` and `arr[i]` being the same idea should feel obvious, because that is exactly what a CUDA kernel does with global memory.
3. **Measure this device's actual peaks** — achievable bandwidth and kernel launch overhead — and record them in `docs/hardware/`. Doable in Python. Every "percentage of peak" claim for the next several years depends on these two numbers, and a roofline built from datasheet figures is worthless.
4. **`fundamentals/01-vector-add` through the full lifecycle** — kernel, correctness test, benchmark, profile, note. Not for the kernel, which is trivial, but because it establishes the template the rest of the repository copies. Write the PyTorch reference first, in the language I already know.

**What to defer deliberately:** C++ classes, templates, RAII, move semantics, the STL. None of them are needed before Phase 10. Reading a C++ book cover to cover before writing a kernel is the most reliable way to never write one.

Update this block whenever the answer to "what do I learn next?" changes. It is the one section of this file that should never be stale.

---

## How to read this roadmap

Every phase is tagged so the next thing to learn is always unambiguous:

| Tag | Meaning |
| --- | --- |
| **REQUIRED NOW** | Blocking. Work on this. |
| **LEARN NEXT** | The immediate horizon. Start when a REQUIRED NOW phase clears. |
| **LEARN LATER** | Real and planned, but not yet. Do not read ahead into it. |
| **OPTIONAL / ADVANCED** | Conditional on earlier results, or genuinely optional. |

Phases overlap. The tags, not the numbers, say what to do now.

**Two phases are cross-cutting and start early despite their numbers:** Phase 4 (profiling) and Phase 13 (benchmarking rigor). Basic competence in both is needed from Phase 3 onward; they are numbered where their *full* treatment belongs.

**The list is deliberately finite.** Anything not in a phase below is out of scope until a phase is added for it. That constraint is what stops this becoming a list of hundreds of topics with no sequence.

---

## PHASE 0 — C and Systems Fundamentals

**REQUIRED NOW** *(the C subset — C++ proper is deferred to Phase 10)*

> **Full structured course: [learning/cpp/phase-00-cpp-foundations/](learning/cpp/phase-00-cpp-foundations/)** — 8 modules, 80 exercises, 8 challenges, 2 mini-projects, cumulative quizzes, and a [CUDA readiness checkpoint](learning/cpp/phase-00-cpp-foundations/cuda-readiness-checkpoint.md) that defines when this phase ends.
>
> Companion material in `c-lessons/`, a separate C track, covers the same memory ideas if that directory is present.

Not months of C++ in isolation. Learn the subset that appears in CUDA kernels, and connect every concept to its GPU counterpart as you go.

**Split, given a start with no C or C++:**

*Now — two weeks, in C:*
- Pointers and pointer arithmetic
- Arrays, memory layout, alignment
- Stack vs heap, `malloc`/`free`; why allocation cost matters in a hot loop
- Structs
- `const`, and `restrict` and what it tells the compiler
- The compilation model: preprocessor, compile, link, translation units
- Cache lines and locality — the direct CPU analogue of memory coalescing

*Later — Phase 10, when writing PyTorch extensions:*
- RAII, classes, templates, move semantics
- The STL subset that appears in extension code
- CMake for multi-file projects

**Connection to GPU work:** cache lines → coalescing. Stack/heap → registers/global memory. False sharing → bank conflicts. A CUDA kernel signature is a C function taking pointers; every hour spent on pointers pays off directly.

**Also in scope:** concurrency vs parallelism, and the cost of data movement between them.

**Deliverable:** naive vs cache-blocked matmul in C, benchmarked, with the difference explained.

**Completion criteria — the C part, blocking on CUDA**
- [ ] Can write a C function that takes pointers and walks an array with index arithmetic without thinking about it
- [ ] Can explain what a pointer holds, what dereferencing does, and where the memory lives
- [ ] Can compile and link a multi-file C program from the command line
- [ ] Cache-blocked matmul implemented in C and benchmarked against the naive version
- [ ] Can explain the speedup in terms of the cache hierarchy, with numbers from my machine

**Completion criteria — the C++ part, deferred to Phase 10**
- [ ] Can read and write the C++ that appears in a CUDA extension
- [ ] Can build a multi-file project with CMake

---

## PHASE 1 — Computer Architecture

**REQUIRED NOW**

**Concepts**
- CPU execution, pipelining, ILP, SIMD
- Cache hierarchy, RAM, latency vs bandwidth
- Memory hierarchy as a cost model
- Arithmetic intensity
- Compute-bound vs memory-bound workloads
- The Roofline model

**The skill this builds:** answering *where is the real bottleneck in this operation?* — which is the question every later phase depends on.

**Deliverable:** a roofline plot for my CPU, built from measured peaks.

**Completion criteria**
- [ ] Can compute arithmetic intensity for an operation by hand
- [ ] Can classify a workload as compute- or memory-bound before running it
- [ ] Measured (not quoted) peak bandwidth and peak FLOP/s for my CPU
- [ ] Roofline plot produced, with my benchmarks placed on it

---

## PHASE 2 — GPU Architecture

**REQUIRED NOW**

The goal is not memorizing architecture. It is being able to connect an observed kernel behaviour to a hardware cause.

**Concepts**
- SMs, CUDA cores, Tensor Cores
- Warps, warp scheduling, SIMT execution
- Blocks, grids, and how they map to hardware
- Registers and register pressure
- Shared memory, L1/L2, global memory, VRAM
- Occupancy, and why maximizing it is not always the goal
- Memory coalescing, warp divergence, memory latency and bandwidth, latency hiding
- Synchronization and atomics
- Ampere GA106 specifically — this device's real limits

**Deliverable:** a measured microbenchmark suite for my GPU: achievable bandwidth, latency, launch overhead. Recorded in `docs/hardware/`.

**Completion criteria**
- [ ] Can explain what happens to a warp on a `__syncthreads()` and on a divergent branch
- [ ] Can predict whether an access pattern coalesces before measuring it
- [ ] Measured achievable bandwidth for my GPU, compared against the spec figure
- [ ] Measured kernel launch overhead
- [ ] Hardware record filled in for the RTX 3060

---

## PHASE 3 — CUDA Fundamentals

**REQUIRED NOW**

Progression, each kernel through correctness → benchmark → profile → understand → optimize → benchmark again:

```text
Vector Add → Memory Coalescing → Matrix Transpose → Shared Memory → Reductions
```

**Concepts**
- Launch model, grids/blocks/threads, indexing
- Global memory access patterns
- Shared memory, tiling, bank conflicts
- Warp primitives: shuffles, ballots, warp reductions
- Synchronization, atomics
- Streams, events, async copies
- Toolchain: `nvcc`, `-Xptxas -v`, PTX, SASS, `cuobjdump`

**Do not copy implementations.** Every one of these has a canonical published version. Write it, measure it, then read the canonical one and explain the difference.

**Deliverable:** `fundamentals/01` through `05`, each with a kernel, a benchmark, a profile and a note.

**Completion criteria**
- [ ] Vector add reaching a documented fraction of measured peak bandwidth
- [ ] Coalescing experiment quantifying the cost of a bad access pattern on this device
- [ ] Transpose with shared memory tiling and bank conflicts removed, both states measured
- [ ] Reduction implemented through the full optimization ladder, each step measured
- [ ] Can explain, for each, what limits it

---

## PHASE 4 — GPU Profiling

**REQUIRED NOW** *(basics)* / **LEARN NEXT** *(full depth)*

Starts alongside Phase 3, because a kernel written without profiling teaches the wrong habits.

```text
Never optimize blindly.
Measure → Profile → Understand → Hypothesize → Optimize → Measure again
```

**Tools**
- **Nsight Systems** — timeline, gaps, launch overhead, host/device overlap. *Use first*, to find what is worth profiling.
- **Nsight Compute** — Speed of Light, memory workload, warp stalls, occupancy, source/SASS correlation.
- **PyTorch Profiler** — operator attribution, framework overhead vs kernel time.
- CUDA events for timing.

**Metrics to read fluently:** kernel duration, memory throughput, achieved bandwidth, occupancy and its limiter, SM utilization, register usage, shared memory usage, memory transactions and sectors per request, warp stall reasons, compute utilization, Tensor Core utilization, launch overhead.

**Deliverable:** profiling scripts in `scripts/profiling/`, and a profile report for every Phase 3 kernel.

**Completion criteria**
- [ ] Can profile a kernel and classify it memory- or compute-bound from Speed of Light
- [ ] Can identify the occupancy limiter — registers, shared memory, or block size
- [ ] Can read warp stall reasons and say what each implies
- [ ] Can find the kernel worth optimizing in a timeline, rather than guessing
- [ ] Every Phase 3 kernel has a report naming a specific limiter

---

## PHASE 5 — Triton

**LEARN NEXT**

After enough CUDA that Triton's abstractions map onto something understood — not before, or the abstraction hides exactly what needs to be learned.

**Concepts**
- Tile/block programming model vs thread-level
- Layouts, block sizes, `num_warps`, `num_stages`
- Memory access and masking
- Reductions in Triton
- Fusion
- Autotuning — and how much it actually buys
- The Triton → GPU mapping, and where the abstraction leaks
- Triton vs CUDA: what each gives up

**Deliverable:** the Phase 3 kernels reimplemented in Triton, benchmarked against the CUDA versions.

**Completion criteria**
- [ ] Same operation implemented in PyTorch, Triton and CUDA, all three benchmarked
- [ ] Can explain the performance difference between the three in mechanical terms
- [ ] Can predict, before running, whether Triton will match CUDA for a given kernel
- [ ] Can identify a case where Triton's abstraction costs something, and say what

---

## PHASE 6 — Deep Learning Kernel Fundamentals

**LEARN NEXT**

The mathematics before the optimization. Optimizing an operation that is not understood mathematically produces fast code and no insight.

**Operations**, in `kernels/` order: activations → softmax → cross entropy.

**Concepts**
- Numerical stability: why softmax subtracts the maximum, what happens when it does not
- Online/streaming reductions
- Error accumulation in reductions
- Which DL operations are memory bound and why almost all of them are

**Deliverable:** `kernels/01` through `03`, forward, through the full lifecycle.

**Completion criteria**
- [ ] Can derive each operation from its definition
- [ ] Can explain each stability measure and demonstrate the failure it prevents
- [ ] Each kernel correct, benchmarked, profiled, bottleneck named

---

## PHASE 7 — Backward Kernels

**LEARN NEXT** — **the central skill of this lab**

The progression this phase exists to produce:

```text
I understand the backward
        ↓
I can implement the backward
        ↓
I can design a high-performance backward kernel
```

**Concepts**
- Reverse-mode autodiff: what a backward kernel actually computes
- Vector-Jacobian products — why the Jacobian is never materialized
- Gradient reductions, and why they run along axes the forward never touches
- Parallel reduction strategies for gradients
- Atomics, race conditions, and deterministic alternatives
- Saved tensors vs recomputation
- Register usage and memory traffic in the backward
- Numerical stability in the backward specifically
- Fusion across the forward/backward boundary

**The trade-off to internalize:**

```text
Save intermediate   vs   Recompute intermediate
```

and the condition under which **doing more FLOPs is faster than moving more data**. This is the idea behind FlashAttention's backward, activation checkpointing, and — possibly — the research North Star.

**Deliverable:** backward kernels for `kernels/01` through `03`, derived by hand first, in `notes/backward-passes/`.

**Completion criteria**
- [ ] Can derive a backward by hand and write it in VJP form
- [ ] Can implement a gradient reduction across the batch, both with atomics and with a two-pass reduction, and say where the crossover is on this device
- [ ] Can state, with numbers, when recompute beats save for a given operation
- [ ] `gradcheck` passing for every kernel implemented so far
- [ ] Can explain what `loss.backward()` does for these operations without invoking it

---

## PHASE 8 — LayerNorm and RMSNorm

**LEARN NEXT** — **research milestone 1 and 2**

Where the roadmap converges on the North Star. Both operations, both directions, fully understood and fully optimized.

**RMSNorm** — forward, RMS computation, scaling, backward derivation, input and weight gradients.
**LayerNorm** — forward, mean, variance, normalization, scale and bias, backward derivation, input/weight/bias gradients.

Then compare them: reductions, memory traffic, numerical stability, parameter gradients, complexity.

**Deliverable:** `kernels/04-layernorm` and `kernels/05-rmsnorm` at stage 10; `projects/03` and `projects/07` written up.

**Completion criteria**
- [ ] Both backwards derived by hand, written up, before implementation
- [ ] PyTorch reference → Triton forward → Triton backward → CUDA forward → CUDA backward, for both
- [ ] Correctness including `gradcheck`
- [ ] Benchmarked across hidden sizes, batch sizes, sequence lengths, and FP32/FP16/BF16
- [ ] Profiled: achieved bandwidth as a fraction of *measured* device peak, forward and backward separately
- [ ] Theoretical minimum traffic computed by hand and compared against measured
- [ ] Bottleneck explained for each
- [ ] The LayerNorm vs RMSNorm comparison written up with evidence

**This clears [Gate 0 and Gate 1](research/paper-roadmap.md).**

---

## PHASE 9 — Kernel Optimization

**LEARN LATER** *(runs continuously alongside Phases 6–12)*

Full catalog: [notes/optimization/](notes/optimization/README.md). Each technique studied in isolation in `experiments/`, then applied to a real kernel **only where the profiler says it is the limiter**.

- **Memory:** coalescing, traffic, shared memory, bank conflicts, vectorized access, tiling, layout, intermediate elimination
- **Execution:** occupancy, register pressure, warp execution, divergence, reductions, synchronization, atomics, latency hiding, launch overhead
- **Compute:** arithmetic intensity, Tensor Cores, mixed precision, FP16/BF16/TF32, later FP8
- **Fusion:** kernel fusion, recomputation, forward/backward fusion boundaries
- **Analysis:** roofline, bound classification, bottleneck taxonomy

**Completion criteria**
- [ ] Each technique measured in isolation on this device, so its size is known
- [ ] Each optimization applied to a real kernel has a hypothesis stated beforehand and a measurement after
- [ ] Failed attempts recorded with the reason the hypothesis was wrong

---

## PHASE 10 — PyTorch Internals

**LEARN LATER** — required before claiming to beat PyTorch

Comparing against a black box is not a comparison. Before saying an implementation is better, know what is actually running.

**Concepts**
- Autograd, `autograd.Function`, `save_for_backward`
- Custom operators, `torch.library`, the dispatcher, ATen
- CUDA extensions: `cpp_extension`, `load_inline`
- `torch.compile`: Dynamo, AOTAutograd, graph breaks, guards
- TorchInductor: graph compilation, fusion decisions, generated Triton
- The caching allocator

**Completion criteria**
- [ ] Can answer "which implementation is PyTorch running here?" for a given op, dtype and shape
- [ ] Custom CUDA op registered with working, gradient-checked autograd
- [ ] Custom op that survives `torch.compile` without a graph break
- [ ] Can read Inductor's generated Triton for a normalization pattern and explain its fusion decisions
- [ ] Can state exactly what a "beat PyTorch" claim is being made against: version, mode, backend, dtype, shape, hardware

---

## PHASE 11 — Fused Normalization Kernels

**LEARN LATER** — **research milestones 3 and 4**

Only after individual kernels are deeply understood. Project: [projects/08](projects/08-fused-normalization/), kernel: [kernels/06](kernels/06-fused-normalization/).

**Prior art first (Gate 2).** Read and benchmark Apex, Liger Kernel, the flash-attention fused layer_norm, and Inductor's output before writing anything. Reproduce their performance before attempting to exceed it.

**Then the questions:**
- Can intermediate materializations be eliminated?
- Can global memory traffic be reduced, and by how much against the theoretical minimum?
- Can kernel launches be reduced?
- Can values stay in registers or shared memory across the fusion boundary?
- Is there a recomputation opportunity existing implementations do not take?
- Is there a backward-specific advantage?

**Fusions:** residual + RMSNorm, RMSNorm + residual, residual + LayerNorm, and other combinations real architectures use.

**Completion criteria**
- [ ] Prior art survey written; existing implementations read as source and benchmarked
- [ ] My implementation within a documented distance of the best of them
- [ ] Fused forward and backward implemented and validated
- [ ] Traffic and launch count measured directly, not inferred from wall clock
- [ ] A specific statement of what existing implementations do not cover — **or an honest conclusion that they cover it all**

---

## PHASE 12 — Transformer Integration

**LEARN LATER** — **research milestone 5**

A microbenchmark result is not a training result.

```text
microbenchmark → operation benchmark → Transformer block → training step → end-to-end training
```

**Measure:** forward latency, backward latency, training step latency, peak memory, throughput, tokens/sec. **And normalization's share of step time** — that share is the ceiling on any possible improvement, and it should be measured before anything is celebrated.

**Completion criteria**
- [ ] Kernel integrated into a real Transformer block
- [ ] Full training step profiled, time attributed across forward, backward and optimizer
- [ ] Normalization's measured share of step time established
- [ ] End-to-end throughput and peak memory measured
- [ ] The kernel-level improvement either still visible end to end, or an explanation of where it was absorbed

**This clears [Gate 4](research/paper-roadmap.md).**

---

## PHASE 13 — Rigorous Benchmarking

**REQUIRED NOW** *(basics)* / **LEARN LATER** *(full rigor)*

Basic discipline from Phase 3 onward; formalized here to publication standard. Full specification: [docs/benchmark-methodology/](docs/benchmark-methodology/README.md).

Warmup · GPU synchronization · multiple runs · mean, median, percentiles, variance · multiple tensor shapes, hidden sizes, batch sizes, sequence lengths · FP32, FP16, BF16 · memory usage · forward, backward, and forward+backward separately · end-to-end training impact.

**Never invent results. Never report only the shape that flatters the implementation.**

**Completion criteria**
- [ ] Harness handles warmup, synchronization, repetition, sweeps and statistics
- [ ] Every result auto-captures hardware and software versions
- [ ] Every claim states version, mode, backend, dtype, shape and hardware
- [ ] Shapes where my implementation loses are reported alongside those where it wins

---

## PHASE 14 — Cross-GPU Validation

**LEARN LATER** *(blocked on cloud hardware)*

```text
RTX 3060 (Ampere consumer) → A100 (Ampere datacenter) → H100 (Hopper)
```

The question is not "is it faster on a bigger GPU". It is whether an optimization is a **general principle** or an **artifact of one memory system**. Both answers are informative.

Only worth paying for after Phase 12 shows a real end-to-end effect.

**Completion criteria**
- [ ] Result reproduced on datacenter Ampere
- [ ] Behaviour on a later architecture characterized
- [ ] Differences explained mechanically, not just reported
- [ ] A clear statement of what the result depends on

---

## PHASE 15 — Research Question Refinement

**OPTIONAL / ADVANCED** *(conditional on Phases 11–14)*

Where the question gets narrowed to something falsifiable, on the basis of measurements rather than expectations. Maintained in [research/research-questions.md](research/research-questions.md).

**Completion criteria**
- [ ] Question narrowed to a specific, measurable, falsifiable claim with a threshold
- [ ] Falsification criteria stated in advance
- [ ] Positioned honestly against prior art
- [ ] **Or:** a documented conclusion that no contribution was found — a valid and complete outcome

---

## PHASE 16 — Paper Experimentation

**OPTIONAL / ADVANCED** *(conditional on Phase 15)*

Systematic experiments designed to answer the refined question, with ablations, controls, and reproducible artifacts.

---

## PHASE 17 — Paper

**OPTIONAL / ADVANCED** *(conditional on Phase 16)*

Written so a knowledgeable reader could reproduce it and disagree with it: full methodology, all shapes including the unfavourable ones, mechanism explained rather than demonstrated, related work positioned honestly, limitations stated, code released.

**A well-explained negative result is a legitimate destination for this phase.**

---

## What is deliberately out of scope

Named so they do not creep back in as "just one more thing":

- **Distributed training** — no hardware; `distributed/` stays empty until cloud
- **FP8** — Ada/Hopper only; not available on this device
- **Neural rendering** — a separate interest, after the core work
- **Inference-specific optimization** — the North Star is training
- **Full compiler engineering** — Inductor is studied as a baseline and a tool, not as a thing to build

Anything not in a phase above is out of scope until a phase is added for it.

---

## Principles

- **Correctness first. Measure second. Optimize third.**
- **Never optimize without measuring.**
- Every concept becomes code, a benchmark, a profiling experiment, or a write-up.
- **A faster kernel is interesting. Understanding why it is faster is more important.**
- **Microbenchmark improvements matter only if they can be connected to real workloads.**
- **Forward performance is not enough. Training requires backward performance.**
- No speedup claimed without a reproducible benchmark and a documented methodology.
- Failed optimizations are recorded, not deleted.
- Depth over breadth: one kernel at stage 10 beats ten at stage 2.
