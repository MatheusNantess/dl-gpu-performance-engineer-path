# Benchmark: <operation> — <date>

<!-- Template. Follows docs/benchmark-methodology/. -->

## Setup

- **Hardware record:** <link to docs/hardware/...>
- **Command:**
- **Seed:**
- **Correctness re-verified before timing:** yes / no  <!-- must be yes -->

## Configuration

- Warmup iterations:
- Measured iterations:
- Timing method: CUDA events / host clock with sync
- Launch overhead included: yes / no
- Clocks locked: yes / no
- TF32 enabled: yes / no
- `torch.compile` mode (if applicable):

## What was compared

<!-- Every variant, and confirmation that they compute the same thing with the
     same numerical semantics. If they do not, say how they differ. -->

## Results — forward

<!-- Median and spread. Multiple shapes, batch sizes, dtypes.
     Include achieved bandwidth or FLOP/s as a percentage of MEASURED peak. -->

| Shape | dtype | Variant | Median | Spread | % of measured peak |
| --- | --- | --- | --- | --- | --- |

## Results — backward

<!-- Separate table. Backward typically moves ~2x the memory of forward. -->

| Shape | dtype | Variant | Median | Spread | % of measured peak |
| --- | --- | --- | --- | --- | --- |

## Where this implementation loses

<!-- Required section. A sweep with the bad shapes removed is a fabrication. -->

## Observations

## What was not tested

<!-- Required. State the limits of the sweep. -->

## Open questions

<!-- Any result that cannot be explained goes here, not in the conclusions. -->