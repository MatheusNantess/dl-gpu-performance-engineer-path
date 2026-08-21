# CUDA Lab

CUDA-specific work, kept parallel in structure to [`triton-lab/`](../triton-lab/README.md) so the same operation can be compared directly across both.

```text
fundamentals/   # language and toolchain mechanics
kernels/        # CUDA implementations of deep learning kernels
experiments/    # CUDA-specific investigations (PTX/SASS, intrinsics, launch config)
```

## When CUDA rather than Triton

Answering this from measurement rather than preference is one of the lab's goals. The current working hypothesis, to be tested:

- **Triton** wins on development speed for elementwise, reduction and tiled kernels, and gets close to hand-written CUDA on most of them.
- **CUDA** is required when the needed control is below Triton's abstraction: specific MMA instruction sequences, unusual memory pipelining, warp specialization, or anything where the generated code needs inspection at SASS level.

Every kernel implemented in both goes into the comparison. The interesting output is not which one wins but *where the crossover is*, and being able to predict it before running.

## Toolchain

`nvcc`, `-Xptxas -v` for register and shared memory usage, `cuobjdump` and `nvdisasm` for SASS, Nsight Compute for source/SASS correlation.