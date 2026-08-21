# Profile: <kernel> — <date>

<!-- Template. Raw captures stay local (gitignored); this interpretation is what gets committed. -->

## Setup

- **Hardware record:** <link to docs/hardware/...>
- **Tool:** Nsight Compute / Nsight Systems / PyTorch Profiler
- **Exact command:**
- **Shapes profiled:**
- **Pass profiled:** forward / backward  <!-- profile them separately -->

## Why this kernel

<!-- What made this kernel worth profiling -- ideally its measured share of step time
     from an Nsight Systems capture. Deep-diving a kernel worth 2% of the step is
     the most common way to waste a week. -->

## Speed of Light

- Memory throughput (% of peak):
- Compute throughput (% of peak):
- **Classification:** memory bound / compute bound / latency bound

## Memory Workload Analysis

- DRAM read / write bytes:
- Theoretical minimum traffic for this algorithm:
- Sectors per request (coalescing):
- Shared memory bank conflicts:

## Warp State Statistics

- Dominant stall reason:
- What it implies:

## Occupancy

- Achieved vs theoretical:
- **Limiter:** registers / shared memory / block size
- Registers per thread:
- Shared memory per block:

## Bottleneck

<!-- One specific limiter, named. "It seems slow" is not a bottleneck analysis. -->

## Hypothesis

<!-- Stated BEFORE any optimization is attempted.
     - What is limiting the kernel:
     - What change should fix it:
     - How much it should gain, and why that number: -->

## Outcome

<!-- Filled in after acting on the hypothesis. Including "the hypothesis was wrong",
     which is a valid and useful outcome. -->

## Open questions