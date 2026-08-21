# 03 — new and delete

## The forms

```cpp
int* p = new int;          // one int
delete p;

int* a = new int[100];     // an array
delete[] a;                // note the []
```

**`new` pairs with `delete`. `new[]` pairs with `delete[]`.** Mismatching them is undefined behaviour.

## Initialization

```cpp
int* a = new int[100];       // UNINITIALIZED -- garbage
int* b = new int[100]();     // zero-initialized
int* c = new int[100]{};     // zero-initialized
```

Plain `new int[100]` gives 100 ints of whatever was in that memory. Same rule as uninitialized locals, and the same danger.

## The three failure modes

**Leak** — allocate, never free:

```cpp
void f() {
    int* p = new int[1000];
}   // p is gone; the memory is not. Nothing can free it now.
```

The pointer went out of scope; the allocation did not. That memory is unreachable and unreleasable for the life of the process.

**Dangling** — free, then use:

```cpp
int* p = new int[10];
delete[] p;
p[0] = 5;         // use after free
```

Often appears to work, because the bytes are still there until the allocator reuses them. Then it stops working, non-deterministically.

**Double free:**

```cpp
delete[] p;
delete[] p;       // undefined behaviour; typically corrupts the allocator
```

## Defensive habit

```cpp
delete[] p;
p = nullptr;      // a later use now crashes loudly instead of silently
```

Converts a silent use-after-free into an immediate segfault. Worth doing every time.

## Allocation failure

`new` throws `std::bad_alloc` on failure. The non-throwing form returns null:

```cpp
int* p = new(std::nothrow) int[huge];
if (p == nullptr) { /* handle it */ }
```

Checking allocation failure is exactly the discipline of checking `cudaMalloc`'s return code — and on a 12 GB device, failure is much more likely than on a host with tens of GB.

## Ownership

The language does not track who is responsible for freeing an allocation. It is a convention, expressed in naming and comments:

```cpp
// Returns a newly allocated array. The CALLER must delete[] it.
float* make_buffer(int n);
```

Getting this wrong produces a leak or a double free. Modern C++ solves it with smart pointers, deliberately out of scope here — because `cudaMalloc`/`cudaFree` has no such help, and the manual discipline is the transferable skill.

## Tools

```bash
g++ -fsanitize=address -g -O0 -o prog prog.cpp && ./prog
valgrind --leak-check=full ./prog
```

Both catch leaks, use-after-free and double frees, with line numbers.

## Why This Matters for CUDA

The mapping is one to one:

```cpp
float* d_data;
cudaMalloc(&d_data, n * sizeof(float));    // new[]
// ... use it ...
cudaFree(d_data);                          // delete[]
```

With three differences that make it harder:

1. **Less memory.** 12 GB, and a training run will use all of it. Leaks surface fast.
2. **No sanitizer by default.** `compute-sanitizer` exists and must be run deliberately.
3. **Errors are asynchronous.** A fault inside a kernel surfaces at the next CUDA call, potentially in unrelated code.

PyTorch's caching allocator exists precisely because `cudaMalloc` is slow and fragmentation is real. Understanding what an allocator does — the topic of this file — is what makes that design comprehensible rather than magic.