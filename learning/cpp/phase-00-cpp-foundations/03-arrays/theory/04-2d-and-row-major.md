# 04 — 2D Arrays and Row-Major Layout

## A 2D array is 1D memory

```cpp
int m[3][4];   // 3 rows, 4 columns
```

Memory is one-dimensional. The compiler stores this as 12 consecutive ints, **row by row** — row-major order.

```text
m[3][4], addresses illustrative, stride 4 bytes

logical:                    memory:

     c0  c1  c2  c3         offset  element
r0 [  0   1   2   3 ]         0     m[0][0]
r1 [  4   5   6   7 ]         1     m[0][1]
r2 [  8   9  10  11 ]         2     m[0][2]
                              3     m[0][3]   <-- end of row 0
                              4     m[1][0]   <-- row 1 begins
                              5     m[1][1]
                              ...
                             11     m[2][3]
```

## The indexing formula

```text
m[row][col]   is at   base + (row * num_cols + col) * sizeof(T)
```

`row * num_cols + col` is **the** formula. It appears in every matrix kernel ever written, and being able to write it without thinking is a genuine prerequisite for Phase 3.

Note it needs `num_cols`, not `num_rows`. Getting that backwards is a classic bug that produces plausible-looking wrong answers.

## Flattening by hand

Real GPU code almost always uses a flat 1D array with manual indexing, rather than a language-level 2D array:

```cpp
float* m = /* rows * cols floats */;
m[row * cols + col] = value;
```

Because: the memory is 1D anyway, `cudaMalloc` returns a flat pointer, and 2D array parameters have awkward rules. Getting comfortable with manual flattening now is not a detour — it is the actual skill.

## Traversal order matters

```cpp
// row-major traversal: walks memory in order
for (int r = 0; r < rows; ++r)
    for (int c = 0; c < cols; ++c)
        sum += m[r * cols + c];

// column-major traversal: jumps by `cols` elements every step
for (int c = 0; c < cols; ++c)
    for (int r = 0; r < rows; ++r)
        sum += m[r * cols + c];
```

Identical results. Identical instruction counts. Very different speeds on a large matrix.

The first reads consecutive addresses, so each cache line fetched gets fully used. The second jumps `cols * sizeof(T)` bytes each step, so every access may pull in a fresh cache line to use one value from it.

**Measuring this difference is one of the exercises**, and it is worth doing rather than believing.

## Row-major versus column-major

C, C++ and NumPy default to row-major. Fortran, MATLAB and cuBLAS use column-major. Mixing conventions is a standard source of transposed results, and cuBLAS in particular catches people out for exactly this reason.

## Why This Matters for CUDA

This module is the direct ancestor of two Phase 3 exercises and one whole class of GPU optimization:

- **Coalescing** is the row-versus-column question with a 32× multiplier. A warp reading along a row is fast; the same warp reading down a column is many times slower.
- **Matrix transpose** is a `fundamentals/` exercise precisely because it *must* be non-coalesced on one side, which forces the use of shared memory as a staging area — the first real GPU optimization technique.
- **Layout choice** stays a live decision all the way up to the research North Star, where how a tensor is laid out determines whether a fused normalization kernel can read it efficiently.

The formula `row * cols + col` never goes away. It just gets an index derived from the thread ID.