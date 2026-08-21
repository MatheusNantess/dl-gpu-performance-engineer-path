# Kernels

The core of this lab. Deep learning kernels implemented from scratch, **forward and backward**, taken through the ten stages in [KERNEL_WORKFLOW.md](../KERNEL_WORKFLOW.md).

## Rule

**Take one kernel to stage 10 before taking the next to stage 2.**

Thirteen kernels at stage 2 is a worse portfolio, and worse learning, than two kernels at stage 10. The stage table below exists to make violations of this rule visible.

## Status

Stages: 1 correctness · 2 forward · 3 backward · 4 validation · 5 benchmark · 6 profile · 7 bottleneck · 8 optimize · 9 vs eager · 10 vs compile/libraries

RT = on the critical path to the [research North Star](../research/north-star.md).

| # | Kernel | Backward | RT | Stage | Status |
| --- | --- | --- | --- | --- | --- |
| 01 | [activations](01-activations/) | Trivial | | — | Not started |
| 02 | [softmax](02-softmax/) | Moderate | | — | Not started |
| 03 | [cross-entropy](03-cross-entropy/) | Easy, subtle | | — | Not started |
| 04 | [layernorm](04-layernorm/) | Hard | **RT** | — | Not started |
| 05 | [rmsnorm](05-rmsnorm/) | Moderate | **RT** | — | Not started |
| 06 | [fused-normalization](06-fused-normalization/) | Hard | **RT** | — | Not started |
| 07 | [fused-activations](07-fused-activations/) | Moderate | | — | Not started |
| 08 | [gemm](08-gemm/) | Easy to derive, hard to make fast | | — | Not started |
| 09 | [attention](09-attention/) | Hard | | — | Not started |
| 10 | [flash-attention](10-flash-attention/) | Very hard | | — | Not started |
| 11 | [fused-mlp](11-fused-mlp/) | Hard | | — | Not started |
| 12 | [transformer-block](12-transformer-block/) | Very hard | **RT** | — | Not started |
| 13 | [fused-transformer-ops](13-fused-transformer-ops/) | Very hard | | — | Not started |

## Ordering

By **backward difficulty**, not forward difficulty. The forward pass of LayerNorm is easier than a fast GEMM; its backward is not.

**06-fused-normalization sits where it does deliberately.** It is the kernel the research North Star is about, and waiting until kernel 12 to touch it would mean years before the central question gets its first measurement. It comes immediately after LayerNorm and RMSNorm because it depends on them and on nothing else.

Kernels 07 through 13 continue the general engineering progression. They are not detours from the research track — GEMM, attention and the fused MLP are what make it possible to say whether a normalization strategy generalizes or is specific to one operation shape.

## Starting a kernel

Copy the canonical layout:

```bash
cp -r kernels/_template kernels/NN-name
```

See [`_template/`](_template/) for the full structure. Directories inside a kernel are created when there is something to put in them. An empty `cuda/` directory sitting there for a year is noise.

## Layout

```text
kernel-name/
├── README.md          # scope, stage checklist, status, links to results
├── forward/           # baseline · triton · cuda
├── backward/          # baseline · triton · cuda
├── correctness/       # forward tests, gradcheck, edge cases, tolerance rationale
├── benchmarks/        # scripts and recorded results
├── profiling/         # profiler output and its interpretation
└── optimization/      # attempts log: hypothesis → change → measurement → verdict
```

## Non-negotiables

1. No timing before correctness passes (stage 4 gates stage 5).
2. Backward is required wherever the operation has one.
3. Forward and backward are benchmarked and profiled **separately** — they have different bottlenecks.
4. Every optimization is preceded by a written, profiling-backed hypothesis.
5. Failed attempts stay in `optimization/`.
6. Every number carries its hardware and software versions.