# Triton Lab

Triton-specific work, kept parallel in structure to [`cuda-lab/`](../cuda-lab/README.md) for direct comparison on the same operations.

```text
fundamentals/   # block-level programming model, masking, autotuning
kernels/        # Triton implementations of deep learning kernels
experiments/    # autotuning studies, generated-code inspection, abstraction limits
```

## What to learn here

- Block-level thinking instead of thread-level: the compiler handles intra-block scheduling
- Program IDs, block pointers, masking for shapes that do not divide evenly
- Autotuning: `num_warps`, `num_stages`, block size selection, and how much it actually buys
- **Writing backward kernels in Triton** — where most public Triton material stops
- Where the abstraction leaks, and what that costs

## Why Triton matters for this role

Most production deep learning kernel work now happens in Triton or in something that generates it. Inductor emits Triton, so reading Inductor's output is reading Triton. Being fluent in it is not optional for the target role — and being able to say precisely when it is *not* enough is what makes CUDA knowledge valuable on top of it.