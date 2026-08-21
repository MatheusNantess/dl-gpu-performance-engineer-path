# Module 07 — Compilation

The build is not a black box, and treating it as one guarantees confusion the moment `nvcc` appears with its two-stage host/device compilation.

The skill this module builds is narrow and important: **given an error message, know immediately whether it came from the compiler, the linker, or the running program.** Each has a different cause and a different fix.

## Theory

| File | Topic |
| --- | --- |
| [01-the-pipeline.md](theory/01-the-pipeline.md) | Preprocess, compile, assemble, link |
| [02-error-taxonomy.md](theory/02-error-taxonomy.md) | Compile vs link vs run-time errors |
| [03-flags.md](theory/03-flags.md) | Warnings, optimization levels, sanitizers |

## Examples

- [`examples/01-multi-file/`](examples/01-multi-file/) — a working three-file build

## Exercises

7 exercises in [`exercises/`](exercises/). **Most are deliberately broken.** The task is to predict the error category before compiling, then read the actual message and fix it.

## Not covered

CMake. It matters later, for PyTorch extensions in Phase 10. Command-line `g++` first — a build system that automates a process you have never performed by hand is just noise.

## Checkpoint

[checkpoint.md](checkpoint.md)

## Why This Matters for CUDA

`nvcc` compiles the same source **twice** — once for the host and once for the device — then stitches the results together. Consequences:

- Errors can come from the host pass, the device pass, or the linker, and the message does not always make that obvious.
- A `__device__` function declared but never defined produces a *link* error, not a compile error.
- Separate compilation of device code (`-rdc=true`) has its own linking stage with its own failures.
- Device code compiles to PTX, then to SASS for a specific architecture. Reading either is a real profiling skill later.

Optimization flags matter more here than usual: comparing `-O0` and `-O3` output on the CPU is a small lesson; comparing PTX and SASS across optimization settings is a routine part of kernel work.