# Start Here

**Read this before anything else in Phase 0.**

The rest of this phase is written for someone who already programs in a compiled language. If C++ is genuinely new — not just unfamiliar, but new — starting at `01-cpp-basics/theory/` will feel like drinking from a hose, because it teaches C++ *and* the memory model *and* the CUDA connection all at once.

This file is the smaller on-ramp. Come back to the full material after it.

## What to ignore, for now

| Ignore | Why |
| --- | --- |
| Every **"Why This Matters for CUDA"** section | Motivating once C++ is comfortable. Pure extra load before that. Skip them; they will make sense on their own in two months |
| **Modules 02–08** | They do not exist yet. Module 01 only |
| **"Predict before running"** | It tests a mental model you do not have yet. For now: run it, look at it, then try to predict |
| **The questions in each exercise header** | Write the code first. Answer the questions later, if at all |
| **`sizeof`, bytes, memory, precision** | Real topics, wrong week |

That leaves a small amount of material. That is the point.

## The only goal for the next two weeks

> **Write, compile and run a small C++ program without looking anything up.**

Not memory. Not pointers. Not bytes. Not CUDA. Just: declare a variable, use `if`, use `for`, write a function, print something.

Everything else in Phase 0 is built on top of that, and none of it works without it.

## The path

### Step 1 — Watch, do not read

Reading is the harder entry point for most people. Use video for the first pass:

- **CS50 (Harvard), Week 1 — "C"**. Free on YouTube. Covers variables, types, conditions, loops, functions in C, taught for people who have never seen it. This is the closest thing to exactly the right material.
- Then **CS50 Week 2 — "Arrays"**, for functions and the basic shape of a program.

C and C++ differ here only in how you print. Everything CS50 teaches in Week 1 applies directly.

Optional, shorter: **The Cherno**, videos "Variables in C++", "Functions in C++", "Loops in C++".

### Step 2 — Retype the examples

Do not read them. **Type them out by hand**, then run them:

```bash
make run FILE=01-cpp-basics/examples/01-types-and-sizes.cpp
```

Typing code you do not fully understand is a legitimate learning method — the shape gets into your hands before it gets into your head. Ignore the parts about bytes and bandwidth. Watch the mechanics: `#include`, `int main()`, `std::cout`, `<<`, `;`, `{}`, `return 0`.

### Step 3 — Write five tiny programs from an empty file

No template, no copying. Empty file, from scratch, each one compiled and run:

1. Print your name and your age on two separate lines.
2. Read two numbers from `std::cin` and print their sum, difference, product and quotient.
3. Print the numbers 1 to 20, one per line. Then only the even ones.
4. Read a number and print whether it is positive, negative or zero.
5. Write a function `int square(int x)`, call it from `main` with three different values, print each result.

If any of these takes more than one attempt, that is the exercise working. Write the fifth one again the next day, from scratch.

### Step 4 — Only the easy exercises of Module 01

In this order, **ignoring the header questions**:

| File | What to do |
| --- | --- |
| `exercise-01.cpp` | Print the size of all 13 types listed. Nothing else |
| `exercise-03.cpp` | Integer division. Just get the output right |
| `exercise-06.cpp` | Loops. Forward, backward, every third |

Skip 02, 04, 05, and 07 through 12 entirely. They come back later.

## Deferrable vs blocking

Not everything in Phase 0 has the same urgency. Learning a topic before there is a reason to need it is inefficient, so it is worth knowing which parts can honestly wait.

**Safe to defer until CUDA is actually being written** — these become concrete the moment a profiler is on screen, and are close to memorization before that:

- roofline, arithmetic intensity, "percentage of peak bandwidth"
- struct padding, alignment, AoS vs SoA
- floating-point bit layout, mantissa and exponent
- accumulator precision in reductions
- coalescing arithmetic

**Not deferrable, because CUDA does not compile without them:**

| | Why it blocks |
| --- | --- |
| `sizeof(T)` gives bytes, and you multiply by `n` | `cudaMalloc(&p, n * sizeof(float))` is line one of every CUDA program. Get it wrong and the kernel writes out of bounds |
| An array is contiguous | Without this, `data[i]` inside a kernel has no meaning |
| **Pointers** (Module 04) | A kernel *is* a function that receives pointers and writes through them. This one is not negotiable |

The first two are single sentences. **Pointers are the real prerequisite** — do not bundle them with the deferrable list. Bytes can be learned alongside CUDA; pointers have to arrive already understood.

## You are ready to move on when

- [ ] I can open an empty file and write a working C++ program without a reference
- [ ] I can compile and run it from the terminal without looking up the command
- [ ] `for`, `if` and functions feel automatic, not looked-up
- [ ] A compiler error message no longer feels like noise

Then go to [README.md](README.md) and start Module 01 properly — theory included, this time. It will read completely differently.

## If it still feels like too much

The bottleneck is almost always the same: not enough programs written from scratch. Reading and watching feel productive and build very little. The five programs in Step 3, written repeatedly from an empty file, do more than a week of reading.

Volume of small programs beats depth of material, at this stage only.
