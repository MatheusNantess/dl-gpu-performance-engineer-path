# 04 — Pointers in Functions

## Output parameters

```cpp
void get_min_max(const int* data, int n, int* out_min, int* out_max);
```

Inputs are `const`; outputs are non-const pointers the function writes through. The caller owns all the memory:

```cpp
int mn, mx;
get_min_max(data, n, &mn, &mx);
```

This is how a function returns several values without containers, and it is exactly how kernels return anything at all.

## `const` placement

```cpp
const float* p;         // pointer to const float: *p is read-only, p can move
float* const p;         // const pointer to float: p is fixed, *p is writable
const float* const p;   // both fixed
```

The first is the one that matters. Read declarations right to left: `const float* p` — "p is a pointer to a float that is const".

Use `const` on every parameter that is not written. It documents intent, the compiler enforces it, and it enables optimizations.

## Modifying the caller's pointer

Passing a pointer by value copies the address. To change *which* address the caller holds, you need its address:

```cpp
void allocate(int** out) {
    *out = new int[100];
}

int* data = nullptr;
allocate(&data);        // data now points at the allocation
```

`int**` — a pointer to a pointer. This looks alarming and is the same rule applied twice: to modify something, take its address.

**This is precisely `cudaMalloc`:**

```cpp
float* d_data;
cudaMalloc(&d_data, bytes);     // signature takes void**
```

## Returning pointers

Safe when the memory outlives the function:

```cpp
int* find(int* data, int n, int target) {
    for (int i = 0; i < n; ++i)
        if (data[i] == target) return &data[i];
    return nullptr;              // not found
}
```

The returned pointer refers to the caller's array, which is still alive.

Unsafe when it refers to a local:

```cpp
int* broken() {
    int x = 42;
    return &x;      // dangling: the frame is gone
}
```

## Ownership

When a function returns heap memory, someone must free it. The language does not track this — it is a convention, documented in comments and function names, and getting it wrong produces either a leak or a double free.

Modern C++ solves this with smart pointers, which are deliberately out of scope here: CUDA memory management is manual `cudaMalloc`/`cudaFree`, and learning the manual discipline first is the point.

## Why This Matters for CUDA

Every element of the standard CUDA pattern is in this file:

```cpp
float* d_input  = nullptr;
float* d_output = nullptr;

cudaMalloc(&d_input,  n * sizeof(float));    // pointer to pointer, so it can be modified
cudaMalloc(&d_output, n * sizeof(float));

cudaMemcpy(d_input, h_input, n * sizeof(float), cudaMemcpyHostToDevice);

vector_scale<<<blocks, threads>>>(d_input, d_output, n, 2.0f);
//                                ^^^^^^^  ^^^^^^^^
//                                const input, writable output

cudaMemcpy(h_output, d_output, n * sizeof(float), cudaMemcpyDeviceToHost);

cudaFree(d_input);       // manual ownership -- no destructor will do this
cudaFree(d_output);
```

Caller-allocated output, explicit sizes, `&` to let a function set a pointer, manual free. All of it is this module.