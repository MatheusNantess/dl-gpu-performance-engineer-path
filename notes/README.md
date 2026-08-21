# Notes

The theory layer. Study notes per domain.

This is the only part of the repo that does not have to become executable immediately — but it is expected to *lead* somewhere:

> Every important concept should eventually become code, a benchmark, a profiling experiment, or a technical write-up.

A note that has sat here for months without producing any of those four is a signal that the topic was interesting rather than useful.

## Directories

| Directory | Contents |
| --- | --- |
| [`optimization/`](optimization/README.md) | **The optimization knowledge base.** Every technique, its cost, and the metric that exposes it |
| [`backward-passes/`](backward-passes/README.md) | **Gradient derivations and backward kernel mechanics.** The priority of this lab |
| `cuda/` | CUDA programming model, toolchain, PTX/SASS |
| `triton/` | Triton language, autotuning, compiler behaviour |
| `gpu-architecture/` | SMs, warps, memory hierarchy, Ampere specifics |
| `computer-architecture/` | Caches, pipelines, ILP, SIMD |
| `cpp/` | The C++ subset that appears in kernel and extension code |
| `pytorch-internals/` | Dispatcher, autograd, allocator, extensions |
| `profiling/` | Profiler mechanics and how to read their output |
| `numerical-computing/` | Floating point, stability, error accumulation |
| `ml-compilers/` | Dynamo, AOTAutograd, Inductor, XLA, TVM, MLIR |
| `distributed-systems/` | Collectives, parallelism strategies |
| `neural-rendering/` | Deferred until the core kernel work is solid |

`optimization/` and `backward-passes/` are the two that matter most for the target role. The rest support them.