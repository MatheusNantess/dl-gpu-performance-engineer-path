# Experiments

Isolated studies of **one optimization technique at a time**, on a problem small enough that the effect of the technique is the only thing being measured.

This is the bridge between [`notes/optimization/`](../notes/optimization/README.md) — where a concept is understood in principle — and [`kernels/`](../kernels/README.md) — where it is applied to something real under profiler guidance.

## Why isolate

In a real kernel, several effects move at once and attribution becomes guesswork. An experiment answers a narrow question with a measurement: *how much does uncoalesced access actually cost on this device?* Knowing the size of an effect in isolation is what makes it possible to predict whether it is worth pursuing in a real kernel.

## Directories

| Directory | Question it answers | Notes |
| --- | --- | --- |
| [`memory-access/`](memory-access/) | What do coalescing, access patterns and vectorized loads cost? | [`notes/optimization/memory/`](../notes/optimization/memory/) |
| [`shared-memory-tiling/`](shared-memory-tiling/) | How much does tiling gain, and what do bank conflicts take back? | [`notes/optimization/memory/`](../notes/optimization/memory/) |
| [`occupancy-and-registers/`](occupancy-and-registers/) | Where is the real occupancy/register/ILP trade-off on this device? | [`notes/optimization/execution/`](../notes/optimization/execution/) |
| [`warp-execution/`](warp-execution/) | What do divergence, shuffles, synchronization and atomics actually cost? | [`notes/optimization/execution/`](../notes/optimization/execution/) |
| [`kernel-fusion/`](kernel-fusion/) | How much traffic and launch overhead does fusion remove? | [`notes/optimization/fusion/`](../notes/optimization/fusion/) |
| [`recomputation/`](recomputation/) | Where does recompute beat store, in bytes vs FLOPs? | [`notes/optimization/fusion/`](../notes/optimization/fusion/) |
| [`tensor-cores/`](tensor-cores/) | What is required to actually hit the MMA pipes, and what is the gain? | [`notes/optimization/compute/`](../notes/optimization/compute/) |
| [`mixed-precision/`](mixed-precision/) | What is the speed gain, and what is the accuracy cost? | [`notes/optimization/compute/`](../notes/optimization/compute/) |
| [`launch-overhead/`](launch-overhead/) | Below what size does launch cost dominate on this device? | [`notes/optimization/execution/`](../notes/optimization/execution/) |
| [`roofline/`](roofline/) | What are this device's *measured* peaks, and where do my kernels sit? | [`notes/optimization/analysis/`](../notes/optimization/analysis/) |

`roofline/` is the one to run first — the measured peaks it produces are what every other experiment and kernel is evaluated against. A roofline built from datasheet numbers is not useful.

## Structure of an experiment

Each experiment directory contains:

```text
experiment-name/
├── README.md      # the question, the setup, the result, the conclusion
├── code/          # minimal reproduction, deliberately small
└── results/       # measurements, with hardware and versions
```

The README states the question **before** the answer, and reports the result even when it contradicts the expectation. An experiment that shows a technique does not help on this device is a useful experiment — it saves the effort of applying it later.

## Rules

- One variable per experiment. Two changes at once measure nothing.
- Every measurement follows [docs/benchmark-methodology/](../docs/benchmark-methodology/README.md).
- Negative and null results are committed. They are often the more valuable ones.
- Results are device-specific and labelled as such.
