# Module 01 — C++ Basics

The point of this module is not syntax. It is the two structural differences between C++ and Python that everything else in this phase rests on:

1. **Types are decided at compile time**, and they determine how many bytes a value occupies.
2. **There is a compile step**, and it is where a whole class of errors is caught — errors Python only discovers at run time, if at all.

Everything else here is detail attached to those two facts.

## Theory

| File | Topic |
| --- | --- |
| [01-program-structure.md](theory/01-program-structure.md) | `main`, includes, the shape of a translation unit |
| [02-types.md](theory/02-types.md) | `int`, `float`, `double`, `bool`, `char` — and their sizes |
| [03-static-typing.md](theory/03-static-typing.md) | What the compiler knows that Python does not |
| [04-control-flow.md](theory/04-control-flow.md) | Loops and branches, and what differs from Python |
| [05-scope-lifetime.md](theory/05-scope-lifetime.md) | When a variable is born and when it dies |

## Examples

Complete, working programs to read and run. They demonstrate; they do not solve any exercise.

- [`examples/01-types-and-sizes.cpp`](examples/01-types-and-sizes.cpp)
- [`examples/02-integer-behaviour.cpp`](examples/02-integer-behaviour.cpp)

## Exercises

12 exercises in [`exercises/`](exercises/), progressive difficulty. Several ask for a **prediction before running** — that is the part that builds intuition, and skipping it turns the exercise into typing practice.

## Checkpoint

[checkpoint.md](checkpoint.md) — theory, exercises and mastery tracked separately.

## Why This Matters for CUDA

- **Type sizes are not trivia.** A kernel processing 10 million `float`s moves 40 MB; the same kernel on `double` moves 80 MB and runs roughly half as fast, because normalization-style kernels are limited by bytes moved, not by arithmetic. Choosing `float` over `double` is a bandwidth decision.
- **`sizeof(T)` drives every stride calculation** a kernel performs. Pointer arithmetic on the GPU is the same arithmetic as here.
- **Compile-time vs run-time** becomes central: CUDA compiles host and device code separately, and knowing which errors come from which is a daily skill.
- **Integer overflow becomes a real bug**, not a curiosity. Indexing a large tensor with a 32-bit `int` overflows silently and corrupts memory. Production kernels worry about this.