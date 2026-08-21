# Mini Project 2 — Memory Explorer

Investigate memory empirically. Small programs, real observations, written conclusions.

This project has no single correct answer. The deliverable is **what you found and what it means**.

## Method

For each task below:

1. **Predict the result before writing code.** Write the prediction down.
2. Write the smallest program that answers the question.
3. Record the observation.
4. Explain it. If the explanation does not fit the prediction, say what was wrong.

Absolute addresses change every run — that is ASLR. **The differences are the data; the absolute values are noise.**

## Tasks

### Group A — Addresses

- [ ] A1. Print the addresses of several stack variables, in declaration order. Which direction do they go?
- [ ] A2. Do consecutive declarations produce consecutive addresses? If not, what sits between them?
- [ ] A3. Compare addresses across two calls of the same function. Across nested calls.
- [ ] A4. How far apart are the stack, the heap, a global variable, and a string literal?

### Group B — Arrays

- [ ] B1. Confirm the stride between array elements equals `sizeof(T)`, for four types.
- [ ] B2. Are two separately declared arrays adjacent in memory? Is the gap predictable?
- [ ] B3. Where does a 2D array's second row begin, relative to the first?
- [ ] B4. Compare the layout of a stack array with a heap array of the same size.

### Group C — Pointers

- [ ] C1. Print a pointer's value, and the pointer's own address. Confirm they differ.
- [ ] C2. Build a chain: `int**` to `int*` to `int`. Print every level.
- [ ] C3. Show that `&arr[0]`, `arr`, and a pointer assigned from `arr` are the same value.
- [ ] C4. Confirm that all pointer types have the same size, whatever they point to.

### Group D — Heap

- [ ] D1. Are consecutive `new[]` allocations adjacent? Measure the gap and compare it to what you requested.
- [ ] D2. Free an allocation and immediately allocate the same size. Same address?
- [ ] D3. Allocate many small blocks, free every second one, then allocate again. Where does it land?
- [ ] D4. Find this machine's stack limit empirically. Compare to `ulimit -s`.

### Group E — Structs

- [ ] E1. Map every field offset of several structs with `offsetof`. Locate the padding.
- [ ] E2. Reorder fields and re-measure. How much was saved?
- [ ] E3. In an array of structs, measure the stride between one field of consecutive elements.
- [ ] E4. Compare that against the same data stored as separate arrays.

### Group F — Locality (the important one)

- [ ] F1. Time sequential, strided and random traversal of a large array.
- [ ] F2. Find the array size at which the ratios change sharply. What does that size correspond to?
- [ ] F3. Sweep the stride from 1 to 64. Plot or tabulate the timing. Where are the discontinuities?
- [ ] F4. Compare row-major and column-major traversal of a large matrix at three sizes.

## Deliverable

`FINDINGS.md`, with one entry per task:

```markdown
### A1 — Stack address direction

**Predicted:**
**Observed:**
**Explanation:**
**Surprised me because:**
```

The last line is optional and usually the most valuable.

## Guidance

- Use `static_cast<const void*>` to print addresses, and `reinterpret_cast<const char*>` for byte arithmetic.
- Run everything several times. Distinguish what varies from what does not.
- For Group F, warm up, repeat, and report medians — timing methodology matters even here.

## Why This Matters for CUDA

Group F is the CPU cousin of memory coalescing, and F2 and F3 are the cache-hierarchy version of the questions Nsight Compute answers about a kernel.

More generally: this project builds the habit of **investigating rather than assuming**. That habit is what the entire benchmarking and profiling methodology of this repository is built on, and it is easier to build here, where the whole system fits in your head, than later on a GPU with thousands of threads.