# Module 05 — References

A third way to pass an argument. Short module — the idea is small, and the useful part is knowing **when to reach for which** of the three.

## Theory

| File | Topic |
| --- | --- |
| [01-references.md](theory/01-references.md) | What `T&` is and how it differs from a pointer |
| [02-choosing.md](theory/02-choosing.md) | Value, pointer, or reference — picking one |

## Examples

- [`examples/01-three-ways.cpp`](examples/01-three-ways.cpp)

## Exercises

6 exercises in [`exercises/`](exercises/), all built around comparing `f(int)`, `f(int*)` and `f(int&)`.

## Checkpoint

[checkpoint.md](checkpoint.md)

## Why This Matters for CUDA

Mostly by contrast. **Device code uses pointers, not references**, because device memory is manipulated through explicit addresses and because CUDA's C API predates and ignores references.

References appear in the *host* code around kernels — C++ wrappers, PyTorch extension glue, anything holding tensors. So the reason to learn them is: recognizing them when reading host-side code, and understanding why the device side does not use them.

The deeper value is the question itself. *Copy or original?* is the same question at every level: a function argument on the CPU, a kernel parameter, a host-to-device transfer. References are one more answer to it.