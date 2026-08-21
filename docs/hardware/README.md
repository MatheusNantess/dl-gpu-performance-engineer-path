# Hardware

Device records. Every benchmark and profiling result in this repository references one of these, because GPU performance numbers are meaningless without knowing what produced them.

## Why this is strict

- Results are not portable across GPUs. A kernel tuned for Ampere consumer memory bandwidth behaves differently on a data center part with several times the bandwidth and more shared memory per SM.
- Driver, CUDA, PyTorch and Triton versions change kernel selection and code generation. A regression between two runs is often a version change, not a code change.
- Consumer cards throttle under sustained load. A result recorded without noting clock behaviour may not reproduce even on the same machine.

## Record template

Copy this per device, fill it from measurement rather than from a datasheet, and reference it from every result.

```markdown
# <device-name>

## Identity
- GPU:
- Architecture / compute capability:
- VRAM:
- Date recorded:

## Software
- Driver:
- CUDA toolkit:
- PyTorch:
- Triton:
- OS:

## Settings
- Clocks locked:
- TF32 enabled:
- Persistence mode:
- Other processes on the GPU:

## Measured peaks
<!-- Measured, not quoted. These are what every roofline and every
     "percentage of peak" claim in the repo is computed against. -->
- Achievable memory bandwidth (measured):
- FP32 throughput (measured):
- FP16 / BF16 Tensor Core throughput (measured):
- Kernel launch overhead (measured):

## Notes
<!-- Thermal behaviour under sustained load, anything that affects reproducibility. -->
```

## Devices

| Device | Role | Record |
| --- | --- | --- |
| NVIDIA RTX 3060 | Primary lab GPU (Ampere GA106) | [rtx-3060.md](rtx-3060.md) — identity and software recorded 2026-08-15; **measured peaks still empty** |

Cloud devices get added as they are used, particularly for FP8 and multi-GPU work, which this device cannot do.