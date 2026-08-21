# Phase 0 — C++ Foundations

> **New to C++ entirely? Start with [START-HERE.md](START-HERE.md), not this file.**
>
> This document and the modules below assume some prior experience with a compiled language. `START-HERE.md` is a much smaller on-ramp: what to ignore, and the two-week goal of simply writing, compiling and running a program unaided. Come back here after it.

## Objective

Build the foundation CUDA requires. Nothing more.

C++ is not the goal of this lab — it is the language the GPU work happens in. This phase exists to make CUDA readable, not to make me a C++ programmer. The moment I can look at

```cpp
void scale(float* data, int n, float factor)
```

and explain what `data` holds, where the elements live, how `data[i]` is computed, what `data + i` means, and roughly how many bytes the function touches — this phase has done its job and CUDA starts.

**This phase is deliberately incomplete as a C++ education.** That is the design, not a shortcut.

## Why C++?

```text
C++            static types, explicit memory, no runtime holding your hand
  ↓
memory         bytes at addresses, laid out contiguously, with a lifetime
  ↓
pointers       a variable holding an address — the only way to talk about GPU memory
  ↓
CUDA C++       a kernel is a C++ function taking pointers into device memory
  ↓
GPU            32 threads reading 32 addresses at once; the pattern decides the speed
```

Every idea in this phase reappears one level down:

| Here | Later |
| --- | --- |
| Contiguous arrays | Coalesced global memory access |
| `sizeof(T)` and pointer arithmetic | Stride calculations inside kernels |
| Stack vs heap | Registers vs global memory |
| Struct padding | Why AoS layouts waste bandwidth on a GPU |
| Compilation and linking | `nvcc`, host/device compilation, PTX and SASS |
| Pass by value vs by pointer | Copying to device vs operating on device memory |

## Prerequisites

**Python at intermediate level.** This material assumes it and does not re-teach programming.

Nowhere here explains what a variable, a loop, or a function is. What it explains is how those behave *differently* in C++ — where the memory is, when it is copied, when it dies, and what the compiler knows at compile time that Python only learns at runtime.

## Environment

The lab runs in **WSL2 / Ubuntu**. Everything here builds with `g++` on the command line — no IDE, deliberately, because understanding the compiler is part of Module 7.

```bash
g++ -std=c++17 -Wall -Wextra -g -O0 -o program file.cpp
./program
```

There is a `Makefile` at this level for convenience, but compile by hand at least a dozen times first. A build command that feels like magic is a gap that will reappear as soon as `nvcc` shows up.

## Relationship to `c-lessons/`

`c-lessons/` -- a separate C track, if present in this repo -- covers the same memory ideas in **C**, with a read-predict-run method. The overlap is real and intentional:

| | `c-lessons/` | This phase |
| --- | --- | --- |
| Language | C | C++ |
| Method | Read the file, predict, run | Theory → exercises → mastery checkpoint |
| Focus | Memory and addresses, narrowly | The C++ subset CUDA needs, broadly |

**Use both.** `c-lessons/01-memory-and-addresses` is excellent companion material for Modules 04 and 06 here, and its predict-before-running discipline is the right habit. Do not treat them as competing tracks — do the C lesson, then the C++ module, and notice what changed.

## Learning order

Strictly sequential. Each module assumes the previous one.

| # | Module | Why here | Exercises |
| --- | --- | --- | --- |
| 01 | [C++ Basics](01-cpp-basics/) | Static typing and the compile-time/run-time split — the biggest change from Python | 12 |
| 02 | [Functions](02-functions/) | Where copies happen, and where they do not | 8 |
| 03 | [Arrays](03-arrays/) | Contiguous memory: the single most important idea for GPU work | 15 |
| 04 | [Pointers](04-pointers/) | **The core module.** Everything after this depends on it | 20 |
| 05 | [References](05-references/) | The third calling convention, and when to pick which | 6 |
| 06 | [Memory](06-memory/) | Stack, heap, lifetime, and the bugs that come with them | 6 |
| 07 | [Compilation](07-compilation/) | Distinguishing compile, link and run-time failures | 7 |
| 08 | [Structs and Layout](08-structs-layout/) | `sizeof`, alignment, padding — where GPU bandwidth gets wasted | 6 |

Then: [challenges/](challenges/) → [mini-projects/](mini-projects/) → [cuda-readiness-checkpoint.md](cuda-readiness-checkpoint.md).

[review/](review/) runs *throughout*, not at the end. Its quizzes are cumulative on purpose.

## How to work through a module

The order inside each module, and what each part is actually for:

```text
1. theory/     Read it. Short files, one session each.
                  ↓
2. examples/   Complete, working programs. PREDICT the output, then run,
               then account for every difference.
                  ↓
3. exercises/  Broken or empty. You write the code. No solutions provided.
                  ↓
4. checkpoint.md   Tick honestly. Mastery is the only line that matters.
```

**`theory/` is where you start.** Nothing else assumes anything you have not read there.

**`examples/` are not exercises and not solutions.** They are complete programs, written to be *read and predicted*, that demonstrate a concept the theory just described. They exist because reading "an `int` is 4 bytes and a pointer is 8" is not the same as watching your own machine print those numbers, and because being wrong about output you were confident about is the fastest way to fix a broken mental model.

Use them like this:

1. Read the code without running it.
2. Write down the expected output — on paper or in a comment.
3. Compile and run.
4. **Account for every difference.** Any surprise is a gap in the model, and that gap is exactly what the example was for.

They deliberately do not solve any exercise. An example demonstrates the concept; the exercises apply it to something else.

**`exercises/` come last** and contain no solutions anywhere in this repository. If one is genuinely stuck, ask for a hint rather than an answer — one hint, then another if needed. A solution handed over early costs the exercise its entire value.

The three dimensions below are the same idea stated as a checklist.

## The three dimensions

A topic is not learned because the theory was read. Every module tracks three separate things:

| | Meaning |
| --- | --- |
| 📘 **Theory** | I read it and followed the argument |
| 💻 **Exercises** | I wrote code that works |
| 🔬 **Mastery** | I can explain it, predict behaviour before running, spot the bug, and apply it to a problem I have not seen |

**Mastery is the only one that counts for moving on.** Theory and exercises are how it gets there. Every module has a `checkpoint.md` that separates the three, and dishonest ticking only costs me time later.

## Completion criteria

### C++ Basics
- [ ] 📘 Theory
- [ ] 💻 Exercises
- [ ] 🔬 Mastery

### Functions
- [ ] 📘 Theory
- [ ] 💻 Exercises
- [ ] 🔬 Mastery

### Arrays
- [ ] 📘 Theory
- [ ] 💻 Exercises
- [ ] 🔬 Mastery

### Pointers
- [ ] 📘 Theory
- [ ] 💻 Exercises
- [ ] 🔬 Mastery

### References
- [ ] 📘 Theory
- [ ] 💻 Exercises
- [ ] 🔬 Mastery

### Memory
- [ ] 📘 Theory
- [ ] 💻 Exercises
- [ ] 🔬 Mastery

### Compilation
- [ ] 📘 Theory
- [ ] 💻 Exercises
- [ ] 🔬 Mastery

### Structs / Layout
- [ ] 📘 Theory
- [ ] 💻 Exercises
- [ ] 🔬 Mastery

### CPU Array Kernel Lab
- [ ] 💻 Completed
- [ ] 🔬 Can explain memory behavior

### CUDA Readiness
- [ ] Theory test
- [ ] Memory reasoning
- [ ] Debugging
- [ ] Coding test
- [ ] Explanation test

## Out of scope for this phase

Named explicitly so they do not creep in and block the entry to CUDA:

inheritance · virtual functions · polymorphism · design patterns · advanced templates · template metaprogramming · smart pointer internals · deep move semantics · most of the STL · multithreading · SIMD intrinsics · CMake · GUI, web, networking, game development

None of these block CUDA. Some arrive in Phase 10, with PyTorch extensions. The rest may never be needed here.

## Definition of done

Phase 0 does not end because every file was read. It ends when I can demonstrate all sixteen:

- [ ] 1. Write small C++ programs unaided
- [ ] 2. Compile without an IDE
- [ ] 3. Read and interpret basic compiler errors
- [ ] 4. Use functions comfortably
- [ ] 5. Understand arrays
- [ ] 6. Understand contiguous memory
- [ ] 7. Understand addresses
- [ ] 8. Understand pointers
- [ ] 9. Distinguish `p`, `*p` and `&p` without hesitating
- [ ] 10. Use pointer arithmetic
- [ ] 11. Understand stack vs heap
- [ ] 12. Understand references
- [ ] 13. Work with structs
- [ ] 14. Reason about `sizeof` and alignment
- [ ] 15. Debug simple memory errors
- [ ] 16. Explain what my code is doing **in terms of memory**

## CUDA readiness

**The goal is not to finish C++.** Nobody finishes C++.

The goal is to reach the point where CUDA becomes learnable, then start CUDA and keep learning C++ alongside it — driven by what the kernel work actually demands. Continuing to study C++ after this bar is met is a way of postponing the real work.

The bar itself is in [cuda-readiness-checkpoint.md](cuda-readiness-checkpoint.md). It is deliberately concrete: if I cannot write `vector_add` on the CPU unaided and explain the memory involved, I am not ready. If I can, I am — regardless of how much C++ I have not learned.

## Log

Record sessions in [learning-log.md](learning-log.md). It has its own template, including a *"Python → C++ differences I noticed"* section and a *"Connections to CUDA"* section — both matter more than the exercise count.
