# 02 — `sizeof` and Element Counts

## `sizeof` on an array

```cpp
int x[4] = {10, 20, 30, 40};

sizeof(x)       // 16 -- the whole array, in bytes
sizeof(x[0])    // 4  -- one element
sizeof(x) / sizeof(x[0])    // 4 -- the element count
```

That division is the standard idiom for recovering a length the array does not store.

## It only works where the array is declared

```cpp
void f(int arr[]) {
    sizeof(arr);   // 8, not 16 -- this is a POINTER now
}
```

Inside a function the array has decayed to a pointer, so `sizeof` reports the pointer's size. Theory file 03 covers this; it is the single most common `sizeof` mistake.

## The layout arithmetic

For an array `T a[n]`:

```text
total bytes        = n * sizeof(T)
address of a[i]    = address of a[0] + i * sizeof(T)
bytes between
consecutive        = sizeof(T)     <-- the STRIDE
elements
```

That middle line is the whole of pointer arithmetic, written out. It is also, unchanged, how a GPU thread computes the address it will read.

```text
float f[4];         stride 4 bytes

  [0]      [1]      [2]      [3]
+--------+--------+--------+--------+
| 4 byte | 4 byte | 4 byte | 4 byte |
+--------+--------+--------+--------+
0x2000   0x2004   0x2008   0x200C

double d[4];        stride 8 bytes

  [0]               [1]               [2]
+-----------------+-----------------+-----------------+
|     8 bytes     |     8 bytes     |     8 bytes     |
+-----------------+-----------------+-----------------+
0x3000            0x3008            0x3010
```

Same element count, twice the bytes, twice the memory traffic.

## Bytes, not elements, is the performance unit

An array of 10 million floats is 40 MB. The same count of doubles is 80 MB. For a kernel limited by memory bandwidth — which is most deep learning kernels — that is roughly a factor of two in runtime.

This is why deep learning runs in FP32 rather than FP64, and why FP16 and BF16 exist. It is a bandwidth argument, and it is this arithmetic.

## `size_t`

`sizeof` yields `size_t`, an unsigned type wide enough to express any object's size. Mixing it with signed `int` in comparisons produces the sign-conversion bug from Module 01. `-Wall -Wextra` warns; listen.

## Why This Matters for CUDA

Every CUDA memory call is in bytes, not elements:

```cpp
cudaMalloc(&d_data, n * sizeof(float));
cudaMemcpy(d_data, h_data, n * sizeof(float), cudaMemcpyHostToDevice);
```

Getting that multiplication wrong allocates too little and produces out-of-bounds writes on the device — which do not crash, and corrupt whatever was next in memory.

Later, `n * sizeof(float)` also becomes the *theoretical minimum traffic* for a kernel that touches each element once. Comparing that number against what Nsight Compute reports as actual DRAM traffic is one of the most useful measurements in this entire lab. It starts here.