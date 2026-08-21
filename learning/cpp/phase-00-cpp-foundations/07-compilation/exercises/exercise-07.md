# Exercise 07 — Optimization flags
**Difficulty: HARD**

Not a broken file. An investigation.

## Goal

Find out what optimization actually does, and why benchmarking at `-O0` is meaningless.

## Requirements

Write a small program with a compute-heavy loop — summing a large array, or a triple-nested matrix multiply is ideal. Then:

1. Build it at `-O0`, `-O1`, `-O2`, `-O3`. Time each. Warm up, repeat, report medians.
2. Record the runtimes in a table.
3. Generate assembly at `-O0` and at `-O2`:
   ```bash
   g++ -S -O0 prog.cpp -o prog-O0.s
   g++ -S -O2 prog.cpp -o prog-O2.s
   wc -l prog-O0.s prog-O2.s
   ```
   Compare the line counts. Then read enough of each to see what changed.
4. Write a loop whose result is never used. Check at `-O2` whether it was removed entirely — time it and see.
5. Compare `-O3` against `-Ofast` on a floating-point reduction. **Check whether the numerical result changed.**

## Questions before starting

1. Predict the `-O0` to `-O2` speedup before measuring. Write the number down.
2. Why is the assembly shorter at `-O2` in some places and longer in others?
3. Why can the compiler delete a loop whose result is unused? What does that imply for naive benchmarks?
4. `-Ofast` enables `-ffast-math`, permitting reassociation of floating-point operations. Why is that a correctness decision rather than a speed knob?

## Record

| Flag | Median time | Speedup vs -O0 | Result value |
| --- | --- | --- | --- |
| `-O0` | | 1.0× | |
| `-O1` | | | |
| `-O2` | | | |
| `-O3` | | | |
| `-Ofast` | | | |

## Why this matters for CUDA

`nvcc` has the same optimization levels plus `--use_fast_math`, which is the GPU `-ffast-math` and carries the same accuracy trade-off. Question 5 is a live decision in kernel work, not a curiosity.

Question 4 matters for a different reason: a benchmark whose result is unused can be optimized away entirely, producing an impossibly fast measurement. Avoiding that is part of the benchmarking methodology this whole repository runs on.