# Kernel Workflow

The single most important document in this repository. It defines what working on a kernel means here, and what makes one *done*.

**A kernel is not done because it runs.** It is done when it has passed every stage below that applies to it.

---

## The lifecycle

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

## The ten stages

Every kernel tracks its position through these. The stage list is copied into each kernel's `README.md` as a checklist.

### 1. Correctness

Understand the operation mathematically before writing anything fast. Write a reference implementation in plain PyTorch or NumPy — deliberately slow, obviously correct. This is what everything later is measured against, so it has to be trustworthy.

**Exit criteria:** a reference implementation exists, plus a test comparing it to the framework operation (when one exists) with explicit tolerances.

### 2. Forward implementation

Implement the forward kernel. Start with the naive version that works, in Triton or CUDA. Do not optimize yet — a slow correct kernel is a valid stage-2 result and an invaluable debugging reference later.

**Exit criteria:** forward kernel runs and matches the reference on a range of shapes and dtypes.

### 3. Backward implementation

**The stage that most repositories skip. It is a priority here.**

Derive the gradients by hand first, on paper, before writing code. Write the derivation into the kernel's write-up — for many operations the derivation is where most of the learning lives.

Then implement the backward kernel. Expect it to be harder than the forward:

- Reductions run along different axes than in forward (weight gradients typically reduce across the batch, which the forward never touches).
- Some intermediates must be saved from forward, or recomputed — that trade-off is a real design decision, not an implementation detail.
- Backward often costs roughly twice the forward's memory traffic, so it frequently dominates training time. Optimizing only the forward optimizes the smaller half.
- Numerical error compounds differently; a backward can be wrong in ways that only show up after many training steps.

**Exit criteria:** backward kernel implemented, derivation written down, saved-vs-recomputed decision documented.

### 4. Correctness validation

Forward *and* gradients, against PyTorch:

- Forward: `torch.testing.assert_close` with tolerances stated per dtype and justified.
- Gradients: `torch.autograd.gradcheck` in float64 where feasible, plus comparison against PyTorch's own backward at the working dtype.
- Edge cases: zeros, large magnitudes, values that trigger overflow in a naive formulation, non-contiguous inputs, shapes that do not divide evenly by the block size.

**Exit criteria:** all correctness tests pass. **No timing run happens before this stage passes.** A fast wrong kernel is worth nothing.

### 5. Benchmarking

Follow [docs/benchmark-methodology/](docs/benchmark-methodology/README.md). Forward and backward are benchmarked **separately** and then together — they have different bottlenecks and hiding one behind the other conceals information.

**Exit criteria:** results across multiple shapes, batch sizes and dtypes, with hardware and versions recorded.

### 6. Profiling

Nsight Compute for the kernel itself, Nsight Systems for how it behaves in a real sequence of operations, PyTorch Profiler when integrated into a model. Profile forward and backward separately.

**Exit criteria:** a profile exists and its key metrics are recorded in `profiling/`.

### 7. Bottleneck analysis

Read the profile and answer, in writing: **is this kernel memory bound or compute bound, and what specifically limits it?**

Name the limiter concretely — achieved bandwidth vs peak, warp stall reason, occupancy limiter (registers/shared memory/blocks), bank conflicts, uncoalesced access, low Tensor Core utilization, launch overhead. "It seems slow" is not a bottleneck analysis.

**Exit criteria:** a written hypothesis naming a specific limiter and predicting what fixing it should gain.

### 8. Optimization

Fix the limiter identified in stage 7 — **one change at a time**, re-measuring after each. Optimizations applied in batches cannot be attributed, which means nothing is learned.

Log every attempt, including the ones that did nothing or made things worse. Those are results too, and usually the more interesting ones.

**Exit criteria:** each accepted optimization has a before/after measurement and an explanation of the mechanism.

### 9. Comparison against PyTorch

Against eager mode, forward and backward, across the benchmark shape sweep. Report honestly, including where eager wins.

### 10. Comparison against `torch.compile` and beyond

Against `torch.compile` (default and `max-autotune`), reading the Triton that Inductor generates rather than treating it as a black box. Then, where the operation justifies it, against Triton/CUDA reference implementations and production libraries: cuBLAS, cuDNN, CUTLASS, FlashAttention.

Losing to a production library is the expected outcome for a long time. **A loss that is precisely explained is a better result than a win that is not.**

---

## Per-kernel directory layout

Starting a kernel means copying [`kernels/_template/`](kernels/_template/):

```text
kernel-name/
├── README.md          # scope, stage checklist, current status, links to results
│
├── forward/
│   ├── baseline/      # slow, obviously correct reference
│   ├── triton/
│   └── cuda/
│
├── backward/
│   ├── baseline/      # reference backward, derived by hand
│   ├── triton/
│   └── cuda/
│
├── correctness/       # forward tests, gradcheck, edge cases, tolerance rationale
├── benchmarks/        # benchmark scripts and recorded results
├── profiling/         # profiler outputs and their interpretation
└── optimization/      # attempts log: hypothesis, change, measurement, verdict
```

Not every subdirectory applies to every kernel — an operation with no meaningful backward does not get a `backward/` tree. Directories are created when there is something to put in them, not in advance.

## Stage checklist template

Copied into each kernel's `README.md`:

```markdown
| # | Stage | Status |
|---|-------|--------|
| 1 | Correctness baseline        | ☐ |
| 2 | Forward implementation      | ☐ |
| 3 | Backward implementation     | ☐ |
| 4 | Correctness validation      | ☐ |
| 5 | Benchmarking                | ☐ |
| 6 | Profiling                   | ☐ |
| 7 | Bottleneck analysis         | ☐ |
| 8 | Optimization                | ☐ |
| 9 | vs PyTorch eager            | ☐ |
| 10 | vs torch.compile / libraries | ☐ |
```

## Rules

1. **No timing before correctness passes.** Stage 4 gates stage 5.
2. **Backward is not optional** where the operation has one. A kernel stuck at stage 2 forever is an unfinished kernel, not a finished forward kernel.
3. **One optimization at a time**, each with its own measurement.
4. **Every optimization needs a profiling-backed hypothesis** stated *before* the change.
5. **Failed attempts are committed**, not deleted.
6. **Every number carries its hardware and versions.**
7. **Depth over breadth.** One kernel taken to stage 10 is worth more than ten kernels at stage 2.
