# 03 — Compiler Flags

## The baseline

```bash
g++ -std=c++17 -Wall -Wextra -g -O0 -o program main.cpp
```

Use this for everything in this phase.

| Flag | Effect | Why |
| --- | --- | --- |
| `-std=c++17` | Selects the language standard | Pins the rules; behaviour stops depending on compiler defaults |
| `-Wall` | Common warnings | Most C++ bugs are warnings someone ignored |
| `-Wextra` | More warnings | Including sign comparison, which catches a real class of bug |
| `-g` | Debug symbols | Needed for gdb and for useful sanitizer output |
| `-O0` | No optimization | Source and generated code correspond; debugging works |
| `-o name` | Output name | Otherwise you get `a.out` |

## Optimization levels

| Flag | Meaning |
| --- | --- |
| `-O0` | None. Fast to compile, slow to run, easy to debug. **Default for learning** |
| `-O1` | Basic |
| `-O2` | Standard for release builds |
| `-O3` | Aggressive: more inlining, vectorization |
| `-Ofast` | `-O3` plus relaxed floating-point rules. **Changes numerical results** |

**Never benchmark at `-O0`.** It measures unoptimized code, which no one runs. Several exercises ask for a `-O0` versus `-O2` comparison specifically to make that concrete.

`-Ofast` enabling `-ffast-math` matters here: it permits reassociating floating-point operations, which changes results. In numerical work — this whole lab — that is a correctness decision, not a speed knob.

## Sanitizers

```bash
g++ -fsanitize=address -g -O0 -o prog main.cpp     # memory errors
g++ -fsanitize=undefined -g -O0 -o prog main.cpp   # undefined behaviour
```

AddressSanitizer catches out-of-bounds access, use-after-free, and leaks, with the exact line. UndefinedBehaviorSanitizer catches signed overflow, bad shifts, misaligned access.

They slow the program down, which is fine — they are for correctness, not measurement. **Use them by default in Modules 04 and 06.**

## Stopping partway

```bash
g++ -E main.cpp -o main.i     # preprocessed source
g++ -S main.cpp -o main.s     # assembly
g++ -c main.cpp -o main.o     # object file
```

Worth doing once each. Looking at `-S` output at `-O0` and again at `-O2` shows what optimization actually does, and is the CPU-side rehearsal for reading PTX and SASS later.

## Useful extras

| Flag | Effect |
| --- | --- |
| `-Werror` | Warnings become errors |
| `-Wshadow` | Warn on shadowed variables |
| `-pedantic` | Strict standard conformance |
| `-lm` | Link the math library (rarely needed with g++) |
| `-I dir` | Add an include search path |
| `-L dir` / `-lname` | Library search path / link a library |

## Why This Matters for CUDA

`nvcc` accepts analogous flags and adds its own:

```bash
nvcc -std=c++17 -O3 -arch=sm_86 -lineinfo -o program kernel.cu
```

- `-arch=sm_86` targets a specific GPU architecture. **`sm_86` is Ampere consumer — the RTX 3060.** Compiling for the wrong architecture either fails to run or falls back to slower generic code.
- `-lineinfo` maps machine instructions back to source lines, which is what makes Nsight Compute's source-level view work. Effectively mandatory for profiling.
- `-Xptxas -v` prints register and shared memory usage per kernel — the first number to look at when occupancy is the question.
- `--use_fast_math` is the GPU `-ffast-math`: faster, less accurate, and a correctness decision.

Passing flags to a compiler and understanding what each does — rather than copying a command line — starts here.