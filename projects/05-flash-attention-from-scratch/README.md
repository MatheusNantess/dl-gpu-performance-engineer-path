# 05 — FlashAttention From Scratch

## Goal

Implement FlashAttention from scratch — tiled attention with online softmax and no materialized N×N attention matrix — and verify the actual mechanism behind its speedup on my own hardware.

The central idea is IO-awareness: attention is bound by memory traffic to and from HBM, not by FLOPs. Tiling keeps blocks of Q, K and V in on-chip memory and computes softmax incrementally, so the attention matrix never touches global memory.

## Planned scope

- Work through the online (streaming) softmax formulation until the running-max/running-sum rescaling is fully understood.
- Implement the forward pass with tiling over the sequence dimension.
- Handle causal masking without wasting work on masked-out blocks.
- Tune block sizes against shared memory and register limits on Ampere.
- Implement the backward pass, including recomputation instead of storing the attention matrix.
- Compare against project 04's naive baseline and against PyTorch's fused SDPA backends.

## What "done" means

- Numerically correct against the naive implementation from project 04, across sequence lengths, head dimensions and causal/non-causal settings.
- Benchmarks showing the memory and runtime scaling difference, with the methodology documented.
- Nsight Compute analysis confirming the reduction in global memory traffic — the claim is about IO, so the IO gets measured.
- A write-up covering the derivation, the tiling strategy, and the block-size tuning results.

## Notes

The hardest project in the current plan and the one that most rewards doing it properly. RTX 3060 constraints (shared memory per SM, Ampere Tensor Core shapes) will shape the implementation, and those constraints should be written down rather than worked around silently.

## Status

Not started. Depends on projects 03 and 04.

---

Kernels inside this project follow the ten-stage lifecycle in [KERNEL_WORKFLOW.md](../../KERNEL_WORKFLOW.md): correctness gates timing, backward is required, and every optimization needs a profiling-backed hypothesis stated beforehand.
