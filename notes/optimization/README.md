# Optimization Knowledge Base

The catalog of kernel optimization concepts. Each one is studied here, isolated in an `experiments/` study, and then applied to a real kernel **only when profiling says it is the limiter**.

The rule that makes this a knowledge base instead of a list of tricks:

> An optimization is applied because a profiler metric said it was the bottleneck — never because it is a known technique.

## How to use this

Each topic below has three things: what it is, **what it costs when you get it wrong**, and **which metric exposes it**. That third column is the point — a concept you cannot measure is a concept you cannot optimize against.

Notes go in the subdirectory listed. Experiments that isolate a single technique go in the matching `experiments/` directory. Real applications go in a kernel's `optimization/` directory with a before/after measurement.

> **Metric names vary across Nsight Compute versions and architectures.** Verify with `ncu --query-metrics` before relying on any of them. The sections named below (Speed of Light, Memory Workload Analysis, Warp State Statistics, Launch Statistics, Occupancy) are stable; individual counter names are less so.

---

## Memory — [`memory/`](memory/)

Most deep learning kernels are memory bound. This is where most of the wins are.

| Concept | What it is | Where it shows up |
| --- | --- | --- |
| **Memory coalescing** | Threads in a warp accessing contiguous addresses so the hardware merges them into few transactions | Sectors per request in Memory Workload Analysis; a ratio far above the ideal means wasted bandwidth |
| **Global memory traffic** | Total bytes actually moved to/from DRAM, vs the theoretical minimum the algorithm needs | DRAM read/write bytes vs a hand-computed lower bound — the single most useful number for a memory-bound kernel |
| **Shared memory** | On-chip scratchpad, far lower latency than global; must be managed explicitly | Shared memory throughput; capacity as an occupancy limiter |
| **Bank conflicts** | Threads hitting the same shared memory bank, serializing the access | Shared memory bank conflict counters; classic in transpose and tiled GEMM |
| **Vectorized memory access** | `float4`/128-bit loads to move more bytes per instruction and reduce instruction count | Instruction counts; often lifts a bandwidth-limited kernel closer to peak |
| **Tiling** | Loading blocks into shared memory / registers so data is reused instead of re-fetched | Arithmetic intensity rises; DRAM traffic falls |
| **Layout optimization** | Choosing tensor layouts so access patterns are coalesced and Tensor Core requirements are met | Often a bigger win than anything done inside the kernel |
| **Intermediate tensor elimination** | Not writing intermediates to global memory at all — the main reason fusion works | Compare DRAM traffic before and after fusing |

**Related:** `experiments/memory-access/`, `experiments/shared-memory-tiling/`

---

## Execution — [`execution/`](execution/)

How work actually gets scheduled on the SM.

| Concept | What it is | Where it shows up |
| --- | --- | --- |
| **Occupancy** | Active warps per SM as a fraction of the maximum | Achieved occupancy in Nsight Compute. **Higher is not automatically better** — a low-occupancy kernel with high ILP can beat a high-occupancy one |
| **Register pressure** | Registers per thread; too many limits how many warps fit, too few forces spilling to local memory | `-Xptxas -v` at compile time; local memory traffic and Launch Statistics at runtime |
| **Warp execution** | The warp as the real unit of scheduling; shuffles and warp-level reductions avoid shared memory entirely | Warp State Statistics |
| **Warp divergence** | Threads in a warp taking different branches, forcing serial execution of both paths | Instructions executed per instruction issued — a ratio below 1 means divergence |
| **Reductions** | Combining values across threads/blocks; the canonical optimization ladder (naive → sequential addressing → warp shuffles → grid-stride) | Central to softmax, normalization, and every backward weight gradient |
| **Synchronization** | `__syncthreads()` and friends — necessary, and a stall source when overused | Barrier stall reasons |
| **Atomics** | Serialized updates to shared locations; often the simplest way to write a backward weight gradient, and often the slowest | Contention shows as stalls; compare against a two-pass reduction |
| **Latency hiding** | Keeping enough independent work in flight to cover memory latency | Warp stall reasons: long scoreboard = waiting on memory |
| **Launch overhead** | Fixed per-launch cost, which dominates for small kernels — a major reason to fuse | Nsight Systems timeline gaps; CUDA graphs as a mitigation |

**Related:** `experiments/occupancy-and-registers/`, `experiments/warp-execution/`, `experiments/launch-overhead/`

---

## Compute and precision — [`compute/`](compute/)

| Concept | What it is | Where it shows up |
| --- | --- | --- |
| **Arithmetic intensity** | FLOPs per byte moved — the x-axis of the roofline and the thing fusion actually increases | Computed by hand from the algorithm; validated against measured traffic |
| **Tensor Core utilization** | Fraction of peak on the MMA pipes | Tensor pipe active as a percentage of peak. Requires the right dtype, shapes, and memory layout — silently falls back if any is wrong |
| **Mixed precision** | Computing in FP16/BF16 while accumulating in FP32 | Compare accuracy against an FP32 reference, not just speed |
| **FP16** | Fast, narrow range, needs loss scaling in training | Overflow/underflow in the backward is the usual failure |
| **BF16** | FP32's range with fewer mantissa bits; generally easier for training | Supported on Ampere — usable on the RTX 3060 |
| **TF32** | Ampere's automatic reduced-precision path for FP32 matmul | On by default for some ops; a fair benchmark states whether it is enabled |
| **FP8** | E4M3/E5M2 — requires per-tensor scaling to be usable | **Ada/Hopper only.** Cloud hardware; not available on the 3060 |

**Related:** `experiments/tensor-cores/`, `experiments/mixed-precision/`, `low-precision/`

---

## Fusion — [`fusion/`](fusion/)

The highest-leverage technique for deep learning kernels, because it attacks memory traffic rather than arithmetic.

| Concept | What it is | Where it shows up |
| --- | --- | --- |
| **Kernel fusion** | Combining operations into one kernel so intermediates stay in registers/shared memory | DRAM traffic falls; launch count falls; arithmetic intensity rises |
| **Recomputation** | Recomputing a value in backward instead of storing it from forward — trading FLOPs for memory | The core idea behind FlashAttention's backward and activation checkpointing |
| **Forward/backward fusion boundaries** | Fusion changes what forward must save for backward; the two decisions are coupled | Peak memory vs backward cost — measure both, not one |
| **Fusion limits** | What Inductor will not fuse, and why: reduction shapes, layout changes, data dependencies | Read the generated Triton to find the boundary |

**Related:** `experiments/kernel-fusion/`, `experiments/recomputation/`

---

## Analysis — [`analysis/`](analysis/)

The models that turn profiler output into a decision.

| Concept | What it is | Where it shows up |
| --- | --- | --- |
| **Roofline analysis** | Plotting achieved performance against arithmetic intensity to see which ceiling you are hitting | Requires a *measured* peak for the device, not a datasheet number |
| **Memory-bound vs compute-bound** | The first question about any kernel; determines which half of this document is relevant | Speed of Light section: compare memory throughput against compute throughput |
| **Bottleneck taxonomy** | A checklist for stage 7: bandwidth, latency, occupancy, divergence, bank conflicts, launch overhead, low Tensor Core utilization | Prevents jumping to a favorite optimization before diagnosing |
| **Speed of Light** | Nsight Compute's headline view — achieved percentage of peak compute and peak memory | The first screen to read on any profile |

**Related:** `experiments/roofline/`, `fundamentals/08-roofline-analysis/`, `profiling/`

---

## Deep learning specifics

Things that matter for DL kernels and rarely appear in general CUDA material:

- **Backward is usually the bigger half.** It typically moves around twice the memory of forward. Optimizing only the forward optimizes the smaller half of training time.
- **Weight gradients reduce across the batch** — an axis the forward pass never touches. This reduction is frequently the backward's bottleneck and has no forward analogue.
- **Normalization and softmax are memory bound**, essentially always. Their optimization is a traffic problem, not an arithmetic one.
- **GEMM is compute bound at large sizes and memory bound at small ones.** Which regime a given layer is in depends on the model's actual shapes.
- **Real shapes matter.** Batch sizes, sequence lengths and hidden dimensions from actual models behave differently from round powers of two.

---

## Study order

Optimization is learned by having a problem, not by reading the list top to bottom:

1. **Analysis first** — you cannot optimize what you cannot classify.
2. **Memory second** — where most deep learning kernel wins are.
3. **Execution third** — once traffic is minimal, scheduling starts to matter.
4. **Fusion fourth** — needs a working multi-kernel baseline to fuse.
5. **Compute and precision fifth** — mostly relevant once memory is no longer the limiter.
