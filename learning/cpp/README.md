# C++

C++ is not a goal of this lab. It is the language GPU work happens in, and the material here covers the subset that CUDA requires — no more.

| Phase | Contents |
| --- | --- |
| [phase-00-cpp-foundations](phase-00-cpp-foundations/) | Types, functions, arrays, pointers, references, memory, compilation, structs |

## Scope

Phase 0 stops at the point where CUDA becomes learnable. Deliberately absent: inheritance, polymorphism, templates beyond the basics, most of the STL, smart pointers, move semantics, CMake.

Those arrive later, driven by need. Advanced C++ for PyTorch extensions belongs to Phase 10 of the [roadmap](../../ROADMAP.md), not here — and treating C++ as a prerequisite to be completed before CUDA is the most reliable way to never reach CUDA.

## Related

`c-lessons/`, a separate C track if present in this repo, covers the same memory ideas in **C**, with a read-predict-run method. The overlap with Phase 0's Modules 04 and 06 is intentional; doing both is worth the time.