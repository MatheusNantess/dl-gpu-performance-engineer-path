# Profiling

Profiling is not a stage that happens after optimization. It is what makes optimization something other than guessing.

```text
Measure → Profile → Understand → Form a hypothesis → Optimize → Measure again
```

**No optimization is applied in this repo without a profiling-backed hypothesis stated beforehand.** A change that made things faster for reasons that cannot be explained is logged as an open question, not as a result.

## Tools and what each is for

| Tool | Question it answers | Directory |
| --- | --- | --- |
| **Nsight Compute** | Why is *this kernel* slow? Memory vs compute, stalls, occupancy, bank conflicts, Tensor Core utilization | [`nsight-compute/`](nsight-compute/) |
| **Nsight Systems** | Why is *this sequence of kernels* slow? Timeline gaps, launch overhead, host/device overlap, CPU bottlenecks | [`nsight-systems/`](nsight-systems/) |
| **PyTorch Profiler** | Where does time go *in this model*? Operator attribution, framework overhead vs kernel time, memory timeline | [`pytorch-profiler/`](pytorch-profiler/) |

Order matters. **Nsight Systems first** to find out *which* kernel or gap deserves attention, then Nsight Compute on that kernel. Deep-diving a kernel that accounts for 2% of step time is wasted effort, and it is the most common way to spend a week optimizing nothing.

## Forward and backward are profiled separately

They have different bottlenecks. The backward typically moves about twice the memory of the forward, and its reductions run along different axes. Profiling only the forward, or only a combined step, hides the half that usually matters more.

## What to look at first

In Nsight Compute, roughly in this order:

1. **Speed of Light** — memory throughput vs compute throughput as percentages of peak. This one screen classifies the kernel as memory bound or compute bound and determines everything that follows.
2. **Memory Workload Analysis** — DRAM traffic, sectors per request (coalescing), shared memory bank conflicts.
3. **Warp State Statistics** — stall reasons. Long scoreboard means waiting on memory; barrier means over-synchronization.
4. **Occupancy** — achieved vs theoretical, and *what limits it*: registers, shared memory, or block size. Note that higher occupancy is not automatically better.
5. **Launch Statistics** — registers per thread, shared memory per block, grid dimensions.

The bottleneck taxonomy and what each metric implies: [`notes/optimization/analysis/`](../notes/optimization/analysis/).

> Metric names vary across Nsight Compute versions and architectures. Verify with `ncu --query-metrics` rather than trusting a name copied from a blog post.

## What gets committed

Raw captures are large and are gitignored — `.ncu-rep`, `.nsys-rep`, `.sqlite`, raw traces stay local.

What gets committed to [`reports/`](reports/) is the **interpretation**: the key metrics extracted, the bottleneck identified, the hypothesis formed, and what happened when it was acted on. A report that lists metrics without stating a conclusion is not finished.

Template: [`docs/templates/profiling-report.md`](../docs/templates/profiling-report.md)

## Reproducibility

Every report records the exact profiling command, the shapes profiled, and the hardware and software versions. Profiling commands live in [`scripts/profiling/`](../scripts/profiling/) so a capture can be reproduced rather than remembered.
