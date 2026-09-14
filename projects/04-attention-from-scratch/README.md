# 04 — Attention From Scratch

## Goal

Implement scaled dot-product attention from scratch, in the naive materialized form — and measure exactly how and why it fails as sequence length grows.

This project exists to earn the motivation for FlashAttention. Building the naive version and watching its memory traffic and footprint scale quadratically is a prerequisite for understanding why the tiled version is shaped the way it is.

## Planned scope

- Implement forward attention: QK^T, scaling, masking, softmax, attention-weights × V.
- Handle causal masking correctly.
- Get the softmax numerics right (max subtraction, stability at long sequences).
- Measure memory usage and runtime as a function of sequence length, and show the quadratic term explicitly.
- Profile to establish where the time goes: the matmuls, the softmax, or the traffic between them.
- Backward pass, once the forward is solid.

## What "done" means

- Correct against `torch.nn.functional.scaled_dot_product_attention` for the same math (with SDPA's fused backends disabled where needed to compare like with like).
- Scaling curves for time and memory vs sequence length, measured on my GPU.
- A profiling-backed account of the bottleneck — expected to be the materialization of the N×N attention matrix, but to be confirmed, not assumed.
- A write-up that sets up project 05.

## Notes

The naive implementation is not a throwaway. It is the correctness reference and the baseline that FlashAttention is measured against.

## Status

Not started.

---

Kernels inside this project follow the ten-stage lifecycle in [KERNEL_WORKFLOW.md](../../KERNEL_WORKFLOW.md): correctness gates timing, backward is required, and every optimization needs a profiling-backed hypothesis stated beforehand.
