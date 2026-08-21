# Fundamentals

Core GPU programming exercises. Everything in [`kernels/`](../kernels/README.md) assumes these are understood, and every one of them is a prerequisite that a real deep learning kernel will exercise simultaneously.

Do them in order — each builds on the last.

| # | Exercise | What it teaches |
| --- | --- | --- |
| 01 | `01-vector-add` | Launch model, indexing, and the bandwidth ceiling |
| 02 | `02-memory-coalescing` | What bad access patterns actually cost, measured |
| 03 | `03-matrix-transpose` | Shared memory tiling and bank conflict removal |
| 04 | `04-shared-memory` | Tiling as a general technique |
| 05 | `05-reductions` | The classic optimization ladder, one step at a time |
| 06 | `06-warps` | Warp-level primitives: shuffles, ballots, warp reductions |
| 07 | `07-occupancy` | Occupancy vs registers vs actual performance |
| 08 | `08-roofline-analysis` | Placing a kernel on a *measured* roofline |

## Why these specifically

Every one of them reappears inside a deep learning kernel:

- Reductions are softmax, LayerNorm, RMSNorm, and every backward weight gradient.
- Coalescing and layout decide whether a normalization kernel reaches peak bandwidth.
- Shared memory tiling is GEMM and FlashAttention.
- Warp primitives are how a fast reduction avoids shared memory entirely.
- Occupancy and registers are the constraint that decides FlashAttention block sizes.

Doing these in isolation first means that when they all appear at once in a real kernel, only the combination is new.

## Standard

Each exercise is done to the same standard: correct kernel, benchmark against a reference, profile, and a note explaining where it sits on the roofline and why. Same discipline as `kernels/`, smaller scope.