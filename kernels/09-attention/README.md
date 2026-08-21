# 09 — Attention

**Backward difficulty:** Hard

## Operation

Scaled dot-product attention in its naive form, materializing the full attention matrix, with causal masking.

## Why this kernel

This kernel exists to earn the motivation for FlashAttention. Building the version that scales quadratically in memory, and measuring exactly where and how it fails, is the prerequisite for understanding why the tiled version has the shape it does.

## Backward

The softmax backward composed with two matrix multiplications, and it needs the attention weights. Storing them is what makes this approach untenable at long sequences, which is precisely the observation the next kernel is built on.

## Milestones

| Milestone | Status |
|---|---|
| Forward | Not started |
| Backward | Not started |
| Forward optimization | Not started |
| Backward optimization | Not started |

## Stage checklist

Stages defined in [KERNEL_WORKFLOW.md](../../KERNEL_WORKFLOW.md). No timing before stage 4 passes.

| # | Stage | Status |
|---|-------|--------|
| 1 | Correctness baseline          | ☐ |
| 2 | Forward implementation        | ☐ |
| 3 | Backward implementation       | ☐ |
| 4 | Correctness validation        | ☐ |
| 5 | Benchmarking                  | ☐ |
| 6 | Profiling                     | ☐ |
| 7 | Bottleneck analysis           | ☐ |
| 8 | Optimization                  | ☐ |
| 9 | vs PyTorch eager              | ☐ |
| 10 | vs torch.compile / libraries | ☐ |

## Expected bottleneck

Memory bound, dominated by writing and re-reading the sequence-by-sequence attention matrix. Both time and peak memory should be measured as functions of sequence length so the quadratic term is visible rather than asserted.

Stated as a hypothesis to be checked against a profile, not as a conclusion.

## Results

No measurements yet.

## Status

Not started. Depends on softmax and GEMM. Target of project 04.

Structure follows [`_template/`](../_template/); subdirectories are created when there is something to put in them.