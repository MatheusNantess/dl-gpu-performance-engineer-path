# 03 — Arrays and Pointers

## The identity

```cpp
int a[4] = {10, 20, 30, 40};

a[i]  ==  *(a + i)
```

This is not an analogy. `a[i]` is *defined* as `*(a + i)`. The bracket notation is syntactic sugar over pointer arithmetic.

A curiosity that follows from the definition: since `a + i == i + a`, the expression `i[a]` is legal and means the same thing. Never write it, but knowing why it works confirms the model is right.

## An array name decays to a pointer

```cpp
int  a[4] = {10, 20, 30, 40};
int* p = a;          // no & needed: `a` becomes &a[0]

p[0]    // 10 -- pointers can be indexed
p[2]    // 30
*(p+2)  // 30 -- same thing
a[2]    // 30 -- same thing
*(a+2)  // 30 -- same thing
```

All four forms compile to the same address computation.

## Where the array and the pointer differ

```cpp
int  a[4];
int* p = a;

sizeof(a);   // 16 -- the whole array
sizeof(p);   // 8  -- just an address

a = p;       // ERROR: an array name is not assignable
p = a;       // fine
++p;         // fine
++a;         // ERROR
```

An array **is** the block of memory. A pointer **refers** to a block. The array name is not a variable holding an address; it is the name of the storage, and it converts to an address when used in an expression.

## The same algorithm, both ways

```cpp
// index form
int sum_index(const int* a, int n) {
    int s = 0;
    for (int i = 0; i < n; ++i) s += a[i];
    return s;
}

// pointer form
int sum_pointer(const int* a, int n) {
    int s = 0;
    for (const int* p = a; p < a + n; ++p) s += *p;
    return s;
}
```

Both walk the same addresses in the same order. Several exercises ask for both versions of one algorithm, because switching between them fluently is the actual goal of this module.

## 2D through pointers

For a flat matrix:

```cpp
float* m;   // rows * cols floats

m[r * cols + c]        // index form
*(m + r * cols + c)    // pointer form
```

Row `r` starts at `m + r * cols`, so a row is itself accessible as a pointer:

```cpp
float* row = m + r * cols;
row[c];                       // element (r, c)
```

That decomposition shows up constantly in tiled matrix kernels.

## Why This Matters for CUDA

Kernels receive pointers, never arrays — for the reasons in Module 03. So every access inside a kernel is pointer arithmetic wearing bracket notation:

```cpp
__global__ void scale(float* data, int n, float factor) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) {
        data[i] *= factor;      // *(data + i) *= factor
    }
}
```

Once `data[i]` is automatically read as "the float at `data + i * 4` bytes", questions like *are these 32 threads reading contiguous memory?* become answerable by inspection. That question is the basis of every memory optimization in this repository.